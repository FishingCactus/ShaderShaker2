#include "catch.hpp"
#include "ast/node.h"
#include "ast/printer/hlsl_printer.h"
#include <sstream>

TEST_CASE( "Returns are printed", "[ast][hlsl][printer]" )
{
    SECTION( "Return is printed" )
    {
        AST::ReturnStatement
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "return;\n" );
    }

    SECTION( "Return with value is printed" )
    {
        AST::ReturnStatement
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.m_Expression = std::shared_ptr<AST::Expression>( new AST::VariableExpression( "a" ) );

        node.Visit( printer );

        CHECK( output.str() == "return a;\n" );
    }
}