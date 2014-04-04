#include "catch.hpp"
#include "ast/node.h"
#include "parser_helper.h"


AST::Expression * ParseLiteralExpression( const char * code )
{
    Parser parser( code, strlen( code ) );

    return  parser.m_Parser.literal_value();
}

TEST_CASE( "Literal are parsed", "[parser]" )
{
    AST::Expression * expression = 0;
    AST::LiteralExpression * literal_expression;

    SECTION( "Integer are parsed" )
    {
        expression = ParseLiteralExpression( " 1234 " );

        REQUIRE( expression );

        literal_expression = dynamic_cast<AST::LiteralExpression*>( expression );

        REQUIRE( literal_expression );
        CHECK( literal_expression->m_Type == AST::LiteralExpression::Int );
        CHECK( literal_expression->m_Value == "1234" );

    }

    SECTION( "Float are parsed" )
    {
        expression = ParseLiteralExpression( " 1234.5f " );

        REQUIRE( expression );

        literal_expression = dynamic_cast<AST::LiteralExpression*>( expression );

        REQUIRE( literal_expression );
        CHECK( literal_expression->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression->m_Value == "1234.5f" );
    }

    SECTION( "Float are parsed2" )
    {
        expression = ParseLiteralExpression( " 1234.0 " );

        REQUIRE( expression );

        literal_expression = dynamic_cast<AST::LiteralExpression*>( expression );

        REQUIRE( literal_expression );
        CHECK( literal_expression->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression->m_Value == "1234.0" );
    }

    SECTION( "Float are parsed3" )
    {
        expression = ParseLiteralExpression( " .123f " );

        REQUIRE( expression );

        literal_expression = dynamic_cast<AST::LiteralExpression*>( expression );

        REQUIRE( literal_expression );
        CHECK( literal_expression->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression->m_Value == ".123f" );
    }

    SECTION( "Float are parsed4" )
    {
        expression = ParseLiteralExpression( " .123 " );

        REQUIRE( expression );

        literal_expression = dynamic_cast<AST::LiteralExpression*>( expression );

        REQUIRE( literal_expression );
        CHECK( literal_expression->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression->m_Value == ".123" );
    }

    SECTION( "True is parsed" )
    {
        expression = ParseLiteralExpression( " true " );

        REQUIRE( expression );

        literal_expression = dynamic_cast<AST::LiteralExpression*>( expression );

        REQUIRE( literal_expression );
        CHECK( literal_expression->m_Type == AST::LiteralExpression::Bool );
        CHECK( literal_expression->m_Value == "true" );
    }

    SECTION( "False is parsed" )
    {
        expression = ParseLiteralExpression( " false " );

        REQUIRE( expression );

        literal_expression = dynamic_cast<AST::LiteralExpression*>( expression );

        REQUIRE( literal_expression );
        CHECK( literal_expression->m_Type == AST::LiteralExpression::Bool );
        CHECK( literal_expression->m_Value == "false" );
    }

    delete expression;
}

TEST_CASE( "Arithmetic expressions are parsed", "[parser]" )
{
    AST::Expression * expression = 0;

    SECTION( "Addition is parsed" )
    {
        AST::BinaryOperationExpression * additive_expression;
        const char code[] = "1+2";
        Parser parser( code, sizeof( code ) - 1 );

        expression = parser.m_Parser.additive_expression();

        REQUIRE( expression );

        additive_expression = dynamic_cast<AST::BinaryOperationExpression*>( expression );

        REQUIRE( additive_expression );

        CHECK( additive_expression->m_Operation == AST::BinaryOperationExpression::Addition );
    }

    SECTION( "Subtraction is parsed" )
    {
        AST::BinaryOperationExpression * subtractive_expression;
        const char code[] = " 1 - 2 ";
        Parser parser( code, sizeof( code ) - 1 );

        expression = parser.m_Parser.additive_expression();

        REQUIRE( expression );

        subtractive_expression = dynamic_cast<AST::BinaryOperationExpression*>( expression );

        REQUIRE( subtractive_expression );

        CHECK( subtractive_expression->m_Operation == AST::BinaryOperationExpression::Subtraction );
    }

    delete expression;
}

TEST_CASE( "Variable expressions are parsed", "[parser]" )
{
    AST::VariableExpression * expression = 0;

    SECTION( "Variable is parsed" )
    {
        const char code[] = " a ";
        Parser parser( code, sizeof( code ) - 1 );

        expression = parser.m_Parser.variable_expression();

        REQUIRE( expression );
        CHECK( expression->m_Name == "a" );
        CHECK( !expression->m_SubscriptExpression );
    }

    SECTION( "Indexed variable is parsed" )
    {
        const char code[] = " a[ 1 ] ";
        Parser parser( code, sizeof( code ) - 1 );

        expression = parser.m_Parser.variable_expression();

        REQUIRE( expression );
        CHECK( expression->m_Name == "a" );
        CHECK( expression->m_SubscriptExpression );
        CHECK( dynamic_cast<AST::LiteralExpression*>( &*expression->m_SubscriptExpression ) );
    }

    delete expression;
}

TEST_CASE( "Function call expression are parsed", "[parser]" )
{
    AST::CallExpression * expression = 0;

    SECTION( "Function call without arguments" )
    {
        const char code[] = " a() ";
        Parser parser( code, sizeof( code ) - 1 );

        expression = parser.m_Parser.call_expression();

        REQUIRE( expression );
        CHECK( expression->m_Name == "a" );
        REQUIRE( expression->m_ArgumentExpressionList );
        REQUIRE( expression->m_ArgumentExpressionList->m_ExpressionList.size() == 0 );
    }

    SECTION( "Function call with arguments" )
    {
        const char code[] = " b( 1, a, a( 1 ) ) ";
        Parser parser( code, sizeof( code ) - 1 );

        expression = parser.m_Parser.call_expression();

        REQUIRE( expression );
        CHECK( expression->m_Name == "b" );
        REQUIRE( expression->m_ArgumentExpressionList );
        REQUIRE( expression->m_ArgumentExpressionList->m_ExpressionList.size() == 3 );
    }

    delete expression;
}

TEST_CASE( "Constructor expression are parsed", "[parser]" )
{
    AST::ConstructorExpression * expression = 0;

    SECTION( "Constructor call without arguments" )
    {
        const char code[] = " float4() ";
        Parser parser( code, sizeof( code ) - 1 );

        expression = parser.m_Parser.constructor();

        REQUIRE( expression );
        REQUIRE( expression->m_Type );
        CHECK( expression->m_Type->m_Name == "float4" );
        REQUIRE( expression->m_ArgumentExpressionList );
        REQUIRE( expression->m_ArgumentExpressionList->m_ExpressionList.size() == 0 );
    }

    SECTION( "Constructor call with arguments" )
    {
        const char code[] = " float4( 1, 2, 3, 4 ) ";
        Parser parser( code, sizeof( code ) - 1 );

        expression = parser.m_Parser.constructor();

        REQUIRE( expression );
        REQUIRE( expression->m_Type );
        CHECK( expression->m_Type->m_Name == "float4" );
        REQUIRE( expression->m_ArgumentExpressionList );
        REQUIRE( expression->m_ArgumentExpressionList->m_ExpressionList.size() == 4 );
    }

    delete expression;
}