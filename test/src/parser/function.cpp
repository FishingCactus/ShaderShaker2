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

    SECTION( "Function with argument is parsed" )
    {
        const char code[] = " void test_with_arguments( float a, int b ) {} ";
        Parser parser( code, sizeof( code ) - 1 );

        declaration = parser.m_Parser.function_declaration();

        REQUIRE( declaration );

        CHECK( !declaration->m_Type );
        CHECK( declaration->m_Semantic.empty() );
        CHECK( declaration->m_Name == "test_with_arguments" );
        CHECK( declaration->m_StorageClassTable.empty() );
        CHECK( declaration->m_ArgumentList );
        REQUIRE( declaration->m_ArgumentList->m_ArgumentTable.size() == 2 );

        CHECK( declaration->m_ArgumentList->m_ArgumentTable[ 0 ]->m_Type->m_Name == "float" );
        CHECK( declaration->m_ArgumentList->m_ArgumentTable[ 0 ]->m_Name == "a" );
        CHECK( declaration->m_ArgumentList->m_ArgumentTable[ 0 ]->m_InputModifier == "" );
        CHECK( !declaration->m_ArgumentList->m_ArgumentTable[ 0 ]->m_TypeModifier );
        CHECK( !declaration->m_ArgumentList->m_ArgumentTable[ 0 ]->m_InitialValue );

        CHECK( declaration->m_ArgumentList->m_ArgumentTable[ 1 ]->m_Type->m_Name == "int" );
        CHECK( declaration->m_ArgumentList->m_ArgumentTable[ 1 ]->m_Name == "b" );
        CHECK( declaration->m_ArgumentList->m_ArgumentTable[ 1 ]->m_InputModifier == "" );
        CHECK( !declaration->m_ArgumentList->m_ArgumentTable[ 1 ]->m_TypeModifier );
        CHECK( !declaration->m_ArgumentList->m_ArgumentTable[ 1 ]->m_InitialValue );

    }

    delete declaration;
}

TEST_CASE( "Arguments are parsed", "[parser]" )
{
    AST::Argument * argument = 0;

    SECTION( "Input modifier is parsed" )
    {
        const char code[] = " inout float test ";
        Parser parser( code, sizeof( code ) - 1 );

        argument = parser.m_Parser.argument();

        REQUIRE( argument );

        CHECK( argument->m_Type->m_Name == "float" );
        CHECK( argument->m_Name == "test" );
        CHECK( argument->m_Semantic == "" );
        CHECK( argument->m_InputModifier == "inout" );
        CHECK( !argument->m_TypeModifier );
        CHECK( !argument->m_InitialValue );
    }

    SECTION( "Type modifier is parsed" )
    {
        const char code[] = " const float test ";
        Parser parser( code, sizeof( code ) - 1 );

        argument = parser.m_Parser.argument();

        REQUIRE( argument );

        CHECK( argument->m_Type->m_Name == "float" );
        CHECK( argument->m_Name == "test" );
        CHECK( argument->m_Semantic == "" );
        CHECK( argument->m_InputModifier == "" );
        REQUIRE( argument->m_TypeModifier );
        CHECK( argument->m_TypeModifier->m_Value == "const" );
        CHECK( !argument->m_InitialValue );
    }

    SECTION( "Semantic is parsed" )
    {
        const char code[] = " float test : MyRandomSemantic ";
        Parser parser( code, sizeof( code ) - 1 );

        argument = parser.m_Parser.argument();

        REQUIRE( argument );

        CHECK( argument->m_Type->m_Name == "float" );
        CHECK( argument->m_Name == "test" );
        CHECK( argument->m_Semantic == "MyRandomSemantic" );
        CHECK( argument->m_InputModifier == "" );
        CHECK( !argument->m_TypeModifier );
        CHECK( !argument->m_InitialValue );
    }

    SECTION( "Initial value is parsed" )
    {
        const char code[] = " float test = 1 ";
        Parser parser( code, sizeof( code ) - 1 );

        argument = parser.m_Parser.argument();

        REQUIRE( argument );

        CHECK( argument->m_Type->m_Name == "float" );
        CHECK( argument->m_Name == "test" );
        CHECK( argument->m_Semantic == "" );
        CHECK( argument->m_InputModifier == "" );
        CHECK( !argument->m_TypeModifier );
        REQUIRE( argument->m_InitialValue );
    }

    SECTION( "Interpolation modifier is parsed" )
    {
        const char code[] = " float test : WithSemantic centroid = 1 ";
        Parser parser( code, sizeof( code ) - 1 );

        argument = parser.m_Parser.argument();

        REQUIRE( argument );

        CHECK( argument->m_Type->m_Name == "float" );
        CHECK( argument->m_Name == "test" );
        CHECK( argument->m_Semantic == "WithSemantic" );
        CHECK( argument->m_InputModifier == "" );
        CHECK( argument->m_InterpolationModifier == "centroid" );
        CHECK( !argument->m_TypeModifier );
        REQUIRE( argument->m_InitialValue );
    }

    delete argument;
}