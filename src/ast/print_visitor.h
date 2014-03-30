#ifndef PRINT_VISITOR_H
    #define PRINT_VISITOR_H

    #include "ast/visitor.h"

    namespace AST
    {

        struct Node;
        struct TranslationUnit;
        struct GlobalDeclaration;
        struct VariableDeclaration;
        struct Type;
        struct IntrinsicType;
        struct UserDefinedType;
        struct SamplerType;
        struct Technique;
        struct TypeModifier;
        struct StorageClass;

        class PrintVisitor : public Visitor
        {

        public:
            virtual void Visit( Node & node ) override;
            virtual void Visit( TranslationUnit & translation_unit ) override;
            virtual void Visit( VariableDeclaration & variable__declaration ) override;
            virtual void Visit( IntrinsicType & type ) override;
            virtual void Visit( UserDefinedType & type ) override;
            virtual void Visit( SamplerType & type ) override;
            virtual void Visit( TypeModifier & modifier ) override;
            virtual void Visit( StorageClass & storage_class ) override;
            virtual void Visit( VariableDeclarationBody & body ) override;

        };
    }

#endif