#ifndef HLSL_H
    #define HLSL_H

    namespace AST{ struct TranslationUnit; }

    namespace HLSL
    {
        AST::TranslationUnit * ParseHLSL( const char * filename );
    }

#endif