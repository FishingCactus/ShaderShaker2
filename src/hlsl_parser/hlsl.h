#ifndef HLSL_H
    #define HLSL_H

    namespace AST{ struct TranslationUnit; }
    #include <base/object_ref.h>
    #include <string>

    namespace HLSL
    {
        Base::ObjectRef<AST::TranslationUnit> ParseHLSL( const std::string & filename );
    }

#endif