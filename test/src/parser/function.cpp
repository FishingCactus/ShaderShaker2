#include "catch.hpp"
#include "ast/node.h"
#include "parser_helper.h"


TEST_CASE( "Function are parsed", "[parser]" )
{
    AST::FunctionDeclaration * declaration = 0;

    SECTION( "Simple function is parsed" )
    {
        const char code[] = " void test() {} ";
        Parser parser( code, sizeof( code ) - 1 );

        declaration = parser.m_Parser.function_declaration();

        REQUIRE( declaration );

        CHECK( !declaration->m_Type );
        CHECK( declaration->m_Semantic == "" );
        CHECK( declaration->m_Name == "test" );
        CHECK( !declaration->m_ArgumentList);
        CHECK( declaration->m_StorageClassTable.empty() );
    }

    SECTION( "Function with return is parsed" )
    {
        const char code[] = " int test() {} ";
        Parser parser( code, sizeof( code ) - 1 );

        declaration = parser.m_Parser.function_declaration();

        REQUIRE( declaration );

        CHECK( declaration->m_Type );
        CHECK( declaration->m_Type->m_Name == "int" );
        CHECK( declaration->m_Semantic == "" );
        CHECK( declaration->m_Name == "test" );
        CHECK( !declaration->m_ArgumentList );
        CHECK( declaration->m_StorageClassTable.empty() );
    }

    SECTION( "Function with return and semantic is parsed" )
    {
        const char code[] = " int test() : AnySemantic {} ";
        Parser parser( code, sizeof( code ) - 1 );

        declaration = parser.m_Parser.function_declaration();

        REQUIRE( declaration );

        CHECK( declaration->m_Type );
        CHECK( declaration->m_Type->m_Name == "int" );
        CHECK( declaration->m_Semantic == "AnySemantic" );
        CHECK( declaration->m_Name == "test" );
        CHECK( !declaration->m_ArgumentList );
        CHECK( declaration->m_StorageClassTable.empty() );
    }

    delete declaration;
}