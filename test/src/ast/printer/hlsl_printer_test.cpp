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