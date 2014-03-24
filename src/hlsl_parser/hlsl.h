#ifndef HLSL_H
    #define HLSL_H

    namespace AST{ struct TranslationUnit; }

    AST::TranslationUnit * ParseHLSL( const char * filename );

#endif