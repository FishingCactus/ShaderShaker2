#include "catch.hpp"
#include "ast/node.h"
#include "parser_helper.h"

TEST_CASE( "Iteration statements are parsed", "[parser]" )
{
    AST::Statement * statement = 0;

    SECTION( "While is parsed" )
    {
        AST::WhileStatement * while_statement;
        const char code[] = "while( 1 );";
        Parser parser( code, sizeof( code ) - 1 );

        statement = parser.m_Parser.iteration_statement();

        REQUIRE( statement );

        while_statement = dynamic_cast<AST::WhileStatement*>( statement );

        REQUIRE( while_statement );
        CHECK( while_statement->m_Condition );
        CHECK( while_statement->m_Statement );
    }

    SECTION( "Do while is parsed" )
    {

        AST::DoWhileStatement * do_while_statement;
        const char code[] = " do ; while( 1 );";
        Parser parser( code, sizeof( code ) - 1 );

        statement = parser.m_Parser.iteration_statement();

        REQUIRE( statement );

        do_while_statement = dynamic_cast<AST::DoWhileStatement*>( statement );

        REQUIRE( do_while_statement );
        CHECK( do_while_statement->m_Condition );
        CHECK( do_while_statement->m_Statement );
    }

    delete statement;
}