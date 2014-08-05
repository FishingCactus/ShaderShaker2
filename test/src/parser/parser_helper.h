#ifndef PARSER_HELPER_H
    #define PARSER_HELPER_H

    #include "hlsl_parser/HLSLLexer.hpp"
    #include "hlsl_parser/HLSLParser.hpp"

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

#endif