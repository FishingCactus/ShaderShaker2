#include "catch.hpp"
#include "ast/node.h"
#include "ast/printer/hlsl_printer.h"
#include <sstream>

TEST_CASE( "Literals are printed", "[ast][hlsl][printer]" )
{

    SECTION( "Integers are printed" )
    {
        AST::LiteralExpression
            node( AST::LiteralExpression::Int, "123" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "123" );
    }

    SECTION( "Floats are printed" )
    {
        AST::LiteralExpression
            node( AST::LiteralExpression::Float, "123.4f" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "123.4f" );
    }

    SECTION( "Booleans are printed" )
    {
        AST::LiteralExpression
            node( AST::LiteralExpression::Int, "true" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "true" );
    }
}

TEST_CASE( "Variables are printed", "[ast][hlsl][printer]" )
{

    SECTION( "Simple variables are printed" )
    {
        AST::VariableExpression
            node( "test" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "test" );
    }

    SECTION( "Float are printed" )
    {
        AST::VariableExpression
            node( "test" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.m_SubscriptExpression = std::shared_ptr<AST::Expression>( new AST::LiteralExpression( AST::LiteralExpression::Int, "1" ) );

        node.Visit( printer );

        CHECK( output.str() == "test[1]" );
    }
}

TEST_CASE( "Unary Operators are printed", "[ast][hlsl][printer]" )
{

    SECTION( "+ operator is printed" )
    {
        AST::UnaryOperationExpression
            node( AST::UnaryOperationExpression::Plus, new AST::LiteralExpression( AST::LiteralExpression::Int, "1" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "+1" );
    }

    SECTION( "- operator is printed" )
    {
        AST::UnaryOperationExpression
            node( AST::UnaryOperationExpression::Minus, new AST::LiteralExpression( AST::LiteralExpression::Int, "1" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "-1" );
    }

    SECTION( "! operator is printed" )
    {
        AST::UnaryOperationExpression
            node( AST::UnaryOperationExpression::Not, new AST::LiteralExpression( AST::LiteralExpression::Int, "1" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "!1" );
    }

    SECTION( "~ operator is printed" )
    {
        AST::UnaryOperationExpression
            node( AST::UnaryOperationExpression::BitwiseNot, new AST::LiteralExpression( AST::LiteralExpression::Int, "1" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "~1" );
    }

}

TEST_CASE( "Call expressions are printed", "[ast][hlsl][printer]" )
{

    SECTION( "Simple call is printed" )
    {
        AST::CallExpression
            node( "test", 0 );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "test()" );
    }

    SECTION( "Call with arguments is printed" )
    {
        AST::ArgumentExpressionList
            * argument_list;

        argument_list = new AST::ArgumentExpressionList();
        argument_list->AddExpression( new AST::VariableExpression( "a" ) );
        argument_list->AddExpression( new AST::VariableExpression( "b" ) );
        argument_list->AddExpression( new AST::VariableExpression( "c" ) );

        AST::CallExpression
            node( "test", argument_list );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "test(a, b, c)" );
    }
}

