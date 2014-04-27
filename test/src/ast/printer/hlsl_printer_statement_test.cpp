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

TEST_CASE( "Jumps are printed", "[ast][hlsl][printer]" )
{
    SECTION( "Break is printed" )
    {
        AST::BreakStatement
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "break;\n" );
    }

    SECTION( "Continue is printed" )
    {
        AST::ContinueStatement
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "continue;\n" );
    }

    SECTION( "Discard is printed" )
    {
        AST::DiscardStatement
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "discard;\n" );
    }
}

TEST_CASE( "Empty statement is printed", "[ast][hlsl][printer]" )
{
    AST::EmptyStatement
        node;
    std::ostringstream
        output;
    AST::HLSLPrinter
        printer( output );

    node.Visit( printer );

    CHECK( output.str() == ";\n" );
}

TEST_CASE( "Expression statement is printed", "[ast][hlsl][printer]" )
{
    AST::ExpressionStatement
        node( new AST::CallExpression( "SomeFunction", 0 ) );
    std::ostringstream
        output;
    AST::HLSLPrinter
        printer( output );

    node.Visit( printer );

    CHECK( output.str() == "SomeFunction();\n" );
}

TEST_CASE( "If statements are printed", "[ast][hlsl][printer]" )
{
    SECTION( "If is printed" )
    {
        AST::IfStatement
            node(
                new AST::VariableExpression( "a" ),
                new AST::ReturnStatement(),
                0
                );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "if( a ) return;\n" );
    }

    SECTION( "If with else is printed" )
    {
        AST::IfStatement
            node(
                new AST::VariableExpression( "a" ),
                new AST::ReturnStatement( new AST::VariableExpression( "b" ) ),
                new AST::ReturnStatement( new AST::VariableExpression( "c" ) )
                );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "if( a ) return b;\nelse return c;\n" );
    }

}

TEST_CASE( "While statement is printed", "[ast][hlsl][printer]" )
{

    AST::WhileStatement
        node(
            new AST::VariableExpression( "a" ),
            new AST::ExpressionStatement( new AST::CallExpression( "Function", 0 ) )
            );
    std::ostringstream
        output;
    AST::HLSLPrinter
        printer( output );

    node.Visit( printer );

    CHECK( output.str() == "while( a ) Function();\n" );
}

TEST_CASE( "Do While statement is printed", "[ast][hlsl][printer]" )
{

    AST::DoWhileStatement
        node(
            new AST::VariableExpression( "a" ),
            new AST::ExpressionStatement( new AST::CallExpression( "Function", 0 ) )
            );
    std::ostringstream
        output;
    AST::HLSLPrinter
        printer( output );

    node.Visit( printer );

    CHECK( output.str() == "do Function();\nwhile( a );\n" );
}
