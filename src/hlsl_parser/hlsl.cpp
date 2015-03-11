#include "hlsl.h"

#include "hlsl_parser/HLSLLexer.hpp"
#include "hlsl_parser/HLSLParser.hpp"
#include "ast/node.h"

Base::ObjectRef<AST::TranslationUnit> HLSL::ParseHLSL( const string & filename )
{
    HLSLLexerTraits::InputStreamType input( (ANTLR_UINT8*)filename.c_str(), ANTLR_ENC_8BIT );
    HLSLLexer lexer( &input );
    HLSLLexerTraits::TokenStreamType token_stream( ANTLR_SIZE_HINT, lexer.get_tokSource() );
    HLSLParser parser( &token_stream );

    return parser.translation_unit();
}