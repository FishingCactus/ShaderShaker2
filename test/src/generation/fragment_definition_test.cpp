#include "catch.hpp"
#include "ast/node.h"
#include "../parser/parser_helper.h"
#include "generation/fragment_definition.h"
#include "generation/function_definition.h"
#include <sstream>

TEST_CASE( "Fragment definition are created", "[generation][fragment]" )
{
    SECTION( "Simple function is parsed" )
    {
        const char code[] = "float4 test(): DiffuseColor{}";
        Parser parser( code, strlen( code ) );

        Base::ObjectRef<AST::TranslationUnit> translation_unit( parser.m_Parser.translation_unit() );

        Base::ObjectRef<Generation::FragmentDefinition> fragment_definition;

        Generation::FragmentDefinition::GenerateFragment( fragment_definition, *translation_unit );

        REQUIRE( fragment_definition );

        Base::ObjectRef<Generation::FunctionDefinition> function_definition;
        REQUIRE( fragment_definition->FindFunctionDefinition( function_definition, "test" ) );

        CHECK( function_definition->GetInSemanticSet().size() == 0 );
        CHECK( function_definition->GetInOutSemanticSet().size() == 0 );
        CHECK( function_definition->GetOutSemanticSet().size() == 1 );
        CHECK( function_definition->GetOutSemanticSet().find("DiffuseColor") != function_definition->GetOutSemanticSet().end() );

    }

    SECTION( "Implicit in argument is parsed" )
    {
        const char code[] = "float4 test( float some_value : DiffuseTexCoord ): DiffuseColor{}";
        Parser parser( code, strlen( code ) );

        Base::ObjectRef<AST::TranslationUnit> translation_unit( parser.m_Parser.translation_unit() );

        Base::ObjectRef<Generation::FragmentDefinition> fragment_definition;

        Generation::FragmentDefinition::GenerateFragment( fragment_definition, *translation_unit );

        REQUIRE( fragment_definition );

        Base::ObjectRef<Generation::FunctionDefinition> function_definition;
        REQUIRE( fragment_definition->FindFunctionDefinition( function_definition, "test" ) );

        CHECK( function_definition->GetInOutSemanticSet().size() == 0 );
        CHECK( function_definition->GetInSemanticSet().size() == 1 );
        CHECK( function_definition->GetInSemanticSet().find("DiffuseTexCoord") != function_definition->GetInSemanticSet().end() );
        CHECK( function_definition->GetOutSemanticSet().size() == 1 );
        CHECK( function_definition->GetOutSemanticSet().find("DiffuseColor") != function_definition->GetOutSemanticSet().end() );

    }

    SECTION( "In argument is parsed" )
    {
        const char code[] = "float4 test( in float some_value : DiffuseTexCoord ): DiffuseColor{}";
        Parser parser( code, strlen( code ) );

        Base::ObjectRef<AST::TranslationUnit> translation_unit( parser.m_Parser.translation_unit() );

        Base::ObjectRef<Generation::FragmentDefinition> fragment_definition;

        Generation::FragmentDefinition::GenerateFragment( fragment_definition, *translation_unit );

        REQUIRE( fragment_definition );

        Base::ObjectRef<Generation::FunctionDefinition> function_definition;
        REQUIRE( fragment_definition->FindFunctionDefinition( function_definition, "test" ) );

        CHECK( function_definition->GetInOutSemanticSet().size() == 0 );
        CHECK( function_definition->GetInSemanticSet().size() == 1 );
        CHECK( function_definition->GetInSemanticSet().find("DiffuseTexCoord") != function_definition->GetInSemanticSet().end() );
        CHECK( function_definition->GetOutSemanticSet().size() == 1 );
        CHECK( function_definition->GetOutSemanticSet().find("DiffuseColor") != function_definition->GetOutSemanticSet().end() );

    }

    SECTION( "Out argument is parsed" )
    {
        const char code[] = "void test( out float some_value : DiffuseTexCoord ){}";
        Parser parser( code, strlen( code ) );

        Base::ObjectRef<AST::TranslationUnit> translation_unit( parser.m_Parser.translation_unit() );

        Base::ObjectRef<Generation::FragmentDefinition> fragment_definition;

        Generation::FragmentDefinition::GenerateFragment( fragment_definition, *translation_unit );

        REQUIRE( fragment_definition );

        Base::ObjectRef<Generation::FunctionDefinition> function_definition;
        REQUIRE( fragment_definition->FindFunctionDefinition( function_definition, "test" ) );

        CHECK( function_definition->GetInSemanticSet().size() == 0 );
        CHECK( function_definition->GetInOutSemanticSet().size() == 0 );
        CHECK( function_definition->GetOutSemanticSet().size() == 1 );
        CHECK( function_definition->GetOutSemanticSet().find("DiffuseTexCoord") != function_definition->GetOutSemanticSet().end() );
    }

    SECTION( "Inout argument is parsed" )
    {
        const char code[] = "void test( inout float some_value : DiffuseTexCoord ){}";
        Parser parser( code, strlen( code ) );

        Base::ObjectRef<AST::TranslationUnit> translation_unit( parser.m_Parser.translation_unit() );

        Base::ObjectRef<Generation::FragmentDefinition> fragment_definition;

        Generation::FragmentDefinition::GenerateFragment( fragment_definition, *translation_unit );

        REQUIRE( fragment_definition );

        Base::ObjectRef<Generation::FunctionDefinition> function_definition;
        REQUIRE( fragment_definition->FindFunctionDefinition( function_definition, "test" ) );

        CHECK( function_definition->GetInSemanticSet().size() == 0 );
        CHECK( function_definition->GetOutSemanticSet().size() == 0 );
        CHECK( function_definition->GetInOutSemanticSet().size() == 1 );
        CHECK( function_definition->GetInOutSemanticSet().find("DiffuseTexCoord") != function_definition->GetInOutSemanticSet().end() );
    }

    SECTION( "In, out argument is parsed" )
    {
        const char code[] = "float test( float some_value : DiffuseTexCoord ) : DiffuseTexCoord{}";
        Parser parser( code, strlen( code ) );

        Base::ObjectRef<AST::TranslationUnit> translation_unit( parser.m_Parser.translation_unit() );

        Base::ObjectRef<Generation::FragmentDefinition> fragment_definition;

        Generation::FragmentDefinition::GenerateFragment( fragment_definition, *translation_unit );

        REQUIRE( fragment_definition );

        Base::ObjectRef<Generation::FunctionDefinition> function_definition;
        REQUIRE( fragment_definition->FindFunctionDefinition( function_definition, "test" ) );

        CHECK( function_definition->GetInOutSemanticSet().size() == 0 );
        CHECK( function_definition->GetOutSemanticSet().size() == 1 );
        CHECK( function_definition->GetInSemanticSet().size() == 1 );
        CHECK( function_definition->GetInSemanticSet().find("DiffuseTexCoord") != function_definition->GetInSemanticSet().end() );
        CHECK( function_definition->GetOutSemanticSet().find("DiffuseTexCoord") != function_definition->GetOutSemanticSet().end() );
    }

    SECTION( "All in" )
    {
        const char code[] = "float test( float some_value : DiffuseTexCoord, "
                            "inout float2 a : A, in float3 b : B, out float4 c : C ) : DiffuseTexCoord{}";
        Parser parser( code, strlen( code ) );

        Base::ObjectRef<AST::TranslationUnit> translation_unit( parser.m_Parser.translation_unit() );

        Base::ObjectRef<Generation::FragmentDefinition> fragment_definition;

        Generation::FragmentDefinition::GenerateFragment( fragment_definition, *translation_unit );

        REQUIRE( fragment_definition );

        Base::ObjectRef<Generation::FunctionDefinition> function_definition;
        REQUIRE( fragment_definition->FindFunctionDefinition( function_definition, "test" ) );

        CHECK( function_definition->GetInOutSemanticSet().size() == 1 );
        CHECK( function_definition->GetOutSemanticSet().size() == 2 );
        CHECK( function_definition->GetInSemanticSet().size() == 2 );
        CHECK( function_definition->GetInSemanticSet().find("DiffuseTexCoord") != function_definition->GetInSemanticSet().end() );
        CHECK( function_definition->GetInSemanticSet().find("B") != function_definition->GetInSemanticSet().end() );
        CHECK( function_definition->GetInOutSemanticSet().find("A") != function_definition->GetInOutSemanticSet().end() );
        CHECK( function_definition->GetOutSemanticSet().find("DiffuseTexCoord") != function_definition->GetOutSemanticSet().end() );
        CHECK( function_definition->GetOutSemanticSet().find("C") != function_definition->GetOutSemanticSet().end() );
    }
}