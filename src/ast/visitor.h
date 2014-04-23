#ifndef VISITOR_H
    #define VISITOR_H


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
        struct VariableDeclarationBody;
        struct TextureDeclaration;
        struct SamplerDeclaration;
        struct SamplerBody;
        struct StructDefinition;

        struct LiteralExpression;
        struct VariableExpression;
        struct UnaryOperationExpression;
        struct CallExpression;
        struct ArgumentExpressionList;
        struct Swizzle;
        struct PostfixSuffixCall;
        struct PostfixSuffixVariable;
        struct ConstructorExpression;
        struct ConditionalExpression;
        struct LValueExpression;
        struct PreModifyExpression;
        struct PostModifyExpression;

        class Visitor
        {

        public:
            virtual void Visit( Node & node ) = 0;
            virtual void Visit( TranslationUnit & translation_unit ) = 0;
            virtual void Visit( VariableDeclaration & variable_declaration ) = 0;
            virtual void Visit( IntrinsicType & type ) = 0;
            virtual void Visit( UserDefinedType & type ) = 0;
            virtual void Visit( SamplerType & type ) = 0;
            virtual void Visit( TypeModifier & modifier ) = 0;
            virtual void Visit( StorageClass & storage_class ) = 0;
            virtual void Visit( VariableDeclarationBody & body ) = 0;
            virtual void Visit( TextureDeclaration & declaration ) = 0;
            virtual void Visit( SamplerDeclaration & declaration ) = 0;
            virtual void Visit( SamplerBody & body ) = 0;
            virtual void Visit( StructDefinition & definition ) = 0;
            virtual void Visit( LiteralExpression & expression ) = 0;
            virtual void Visit( VariableExpression & expression ) = 0;
            virtual void Visit( UnaryOperationExpression & expression ) = 0;
            virtual void Visit( CallExpression & expression ) = 0;
            virtual void Visit( ArgumentExpressionList & list ) = 0;
            virtual void Visit( Swizzle & swizzle ) = 0;
            virtual void Visit( PostfixSuffixCall & postfix_suffix ) = 0;
            virtual void Visit( PostfixSuffixVariable & postfix_suffix ) = 0;
            virtual void Visit( ConstructorExpression & expression ) = 0;
            virtual void Visit( ConditionalExpression & expression ) = 0;
            virtual void Visit( LValueExpression & expression ) = 0;
            virtual void Visit( PreModifyExpression & expression ) = 0;
            virtual void Visit( PostModifyExpression & expression ) = 0;
        };
    }

#endif