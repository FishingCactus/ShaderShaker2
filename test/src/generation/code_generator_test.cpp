#include "catch.hpp"
#include <ast/node.h>
#include "../parser/parser_helper.h"
#include <generation/fragment_definition.h>
#include <generation/function_definition.h>
#include <generation/code_generator.h>
#include <base/error_handler_interface.h>
#include <sstream>

#include <ast/printer/hlsl_printer.h>

namespace
{
    struct SimpleErrorHandler : public Base::ErrorHandlerInterface
    {
        virtual void ReportError(
            const std::string & message,
            const std::string & file
            ) override
        {
            m_Message = message;
            m_File = file;
        }

        std::string
            m_Message,
            m_File;
    };

    static AST::TranslationUnit * ParseCode(
        const std::string & code
        )
    {
        Parser parser( code.c_str(), code.length() );

        return parser.m_Parser.translation_unit();
    }

    static std::vector<Base::ObjectRef<Generation::FragmentDefinition> > GetFragmentTable(
        std::vector<std::string> & fragment_source_table
        )
    {
        std::vector<Base::ObjectRef<AST::TranslationUnit> >
            translation_unit_table;

        for(std::string code : fragment_source_table )
        {
            translation_unit_table.push_back(
                ParseCode( code )
                );
        }
        std::vector<Base::ObjectRef<Generation::FragmentDefinition> > definition_table;

        for( auto translation_unit : translation_unit_table )
        {
            Base::ObjectRef<Generation::FragmentDefinition>
                definition;

            Generation::FragmentDefinition::GenerateFragment( definition, *translation_unit );

            definition_table.push_back( definition );
        }

        return definition_table;
    }

}

TEST_CASE( "Special cases are handled", "[generation][fragment]" )
{
    SECTION( "Recovering previoulsy generated semantic" )
    {
        std::vector<Base::ObjectRef<AST::TranslationUnit> >
            translation_unit_table;
        std::vector<std::string>
            code_table =
            {
                "float C( float b :B ) : C { return b; }",
                "float B( float x :X ) : B { return x; }",
                "float A( float c : C, float b :B ) : A { return b + c; }"
            };
        std::vector<Base::ObjectRef<Generation::FragmentDefinition> >
            definition_table;

        definition_table = GetFragmentTable( code_table );

        Generation::CodeGenerator code_generator;
        Base::ObjectRef<SimpleErrorHandler> error_handler = new SimpleErrorHandler;

        Base::ObjectRef<AST::TranslationUnit> translation_unit;

        translation_unit = code_generator.GenerateShader(
            definition_table,
            { "A" },
            { "X" },
            * error_handler
            );

        CHECK( translation_unit );
        CHECK( error_handler->m_Message == "" );
    }
}

TEST_CASE( "Cycle are detected", "[generation][fragment]" )
{
    SECTION( "Direct cycles are detected" )
    {
        std::vector<Base::ObjectRef<AST::TranslationUnit> >
            translation_unit_table;
        std::vector<std::string>
            code_table =
            {
                "float A( float c :C ) : A { return c; }",
                "float B( float a :A ) : B { return a; }",
                "float C( float b :B ) : C { return b; }"
            };
        std::vector<Base::ObjectRef<Generation::FragmentDefinition> >
            definition_table;

        definition_table = GetFragmentTable( code_table );

        Generation::CodeGenerator code_generator;
        Base::ObjectRef<SimpleErrorHandler> error_handler = new SimpleErrorHandler;

        code_generator.GenerateShader(
            definition_table,
            { "A" },
            {},
            * error_handler
            );

        CHECK( error_handler->m_Message == "Cycle detected involving A" );
    }
}
