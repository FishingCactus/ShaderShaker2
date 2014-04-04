#include "catch.hpp"
#include "ast/node.h"
#include "parser_helper.h"

TEST_CASE( "If statement are parsed", "[parser]" )
{
    AST::IfStatement * statement = 0;

    SECTION( "Simple if is parsed" )
    {
        const char code[] = " if( 1 ) ;";
        Parser parser( code, sizeof( code ) - 1 );

        statement = parser.m_Parser.if_statement();

        REQUIRE( statement );
        REQUIRE( statement->m_Condition );
        REQUIRE( statement->m_ThenStatement );
        REQUIRE( dynamic_cast<AST::EmptyStatement*>( &*statement->m_ThenStatement ) );
        CHECK( !statement->m_ElseStatement );
    }

    SECTION( "If then else is parsed" )
    {
        const char code[] = " if( 1 ) ; else ;";
        Parser parser( code, sizeof( code ) - 1 );

        statement = parser.m_Parser.if_statement();

        REQUIRE( statement );
        REQUIRE( statement->m_Condition );
        REQUIRE( statement->m_ThenStatement );
        CHECK( dynamic_cast<AST::EmptyStatement*>( &*statement->m_ThenStatement ) );
        REQUIRE( statement->m_ElseStatement );
        CHECK( dynamic_cast<AST::EmptyStatement*>( &*statement->m_ElseStatement ) );
    }

    SECTION( "If then else if is parsed" )
    {
        const char code[] = " if( 1 ) ; else if( 2 ) ;";
        Parser parser( code, sizeof( code ) - 1 );

        statement = parser.m_Parser.if_statement();

        REQUIRE( statement );
        REQUIRE( statement->m_Condition );
        REQUIRE( statement->m_ThenStatement );
        CHECK( dynamic_cast<AST::EmptyStatement*>( &*statement->m_ThenStatement ) );
        REQUIRE( statement->m_ElseStatement );
        CHECK( dynamic_cast<AST::IfStatement*>( &*statement->m_ElseStatement ) );
    }

    delete statement;
}