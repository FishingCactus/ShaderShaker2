#include "catch.hpp"
#include "hlsl_parser/HLSLLexer.hpp"
#include "hlsl_parser/HLSLParser.hpp"
#include "ast/node.h"

struct Parser
{
    Parser( const char * code, const int size ) :
        m_Input( (ANTLR_UINT8* )code, ANTLR_ENC_8BIT, size, (ANTLR_UINT8*) "" ),
        m_Lexer( &m_Input ),
        m_TokenStream( ANTLR_SIZE_HINT, m_Lexer.get_tokSource() ),
        m_Parser( &m_TokenStream )
    {
    }

    HLSLLexerTraits::InputStreamType
        m_Input;
    HLSLLexer
        m_Lexer;
    HLSLLexerTraits::TokenStreamType
        m_TokenStream;
    HLSLParser
        m_Parser;
};

TEST_CASE( "Arithmetic expression are parsed", "[parser]" )
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