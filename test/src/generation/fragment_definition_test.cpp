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
        CHECK( function_definition->GetOutSemanticSet().find("DiffuseColor") != function_definition->GetInSemanticSet().end() );

    }
}