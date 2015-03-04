#ifndef PRINT_VISITOR_H
    #define PRINT_VISITOR_H

    #include "ast/const_visitor.h"

    namespace AST
    {
        class PrintVisitor : public ConstVisitor
        {

        public:
            virtual void Visit( const Node & node ) override;
            virtual void Visit( const TranslationUnit & translation_unit ) override;
            virtual void Visit( const VariableDeclaration & variable__declaration ) override;
            virtual void Visit( const IntrinsicType & type ) override;
            virtual void Visit( const UserDefinedType & type ) override;
            virtual void Visit( const SamplerType & type ) override;
            virtual void Visit( const TypeModifier & modifier ) override;
            virtual void Visit( const StorageClass & storage_class ) override;
            virtual void Visit( const VariableDeclarationBody & body ) override;
            virtual void Visit( const TextureDeclaration & declaration ) override;
            virtual void Visit( const SamplerDeclaration & declaration ) override;
            virtual void Visit( const SamplerBody & body ) override;
            virtual void Visit( const StructDefinition & definition ) override;

            // Expressions
            virtual void Visit( const LiteralExpression & expression ) override;
            virtual void Visit( const VariableExpression & expression ) override;
            virtual void Visit( const UnaryOperationExpression & expression ) override;
            virtual void Visit( const BinaryOperationExpression & expression ) override;
            virtual void Visit( const CallExpression & expression ) override;
            virtual void Visit( const ArgumentExpressionList & list ) override;
            virtual void Visit( const Swizzle & swizzle ) override;
            virtual void Visit( const PostfixSuffixCall & postfix_suffix ) override;
            virtual void Visit( const PostfixSuffixVariable & postfix_suffix ) override;
            virtual void Visit( const ConstructorExpression & expression ) override;
            virtual void Visit( const ConditionalExpression & expression ) override;
            virtual void Visit( const LValueExpression & expression ) override;
            virtual void Visit( const PreModifyExpression & expression ) override;
            virtual void Visit( const PostModifyExpression & expression ) override;
            virtual void Visit( const CastExpression & expression ) override;
            virtual void Visit( const AssignmentExpression & expression ) override;

            // Statements

            virtual void Visit( const ReturnStatement & statement ) override;
            virtual void Visit( const BreakStatement & statement ) override;
            virtual void Visit( const ContinueStatement & statement ) override;
            virtual void Visit( const DiscardStatement & statement ) override;
            virtual void Visit( const EmptyStatement & statement ) override;
            virtual void Visit( const ExpressionStatement & statement ) override;
            virtual void Visit( const IfStatement & statement ) override;
            virtual void Visit( const WhileStatement & statement ) override;
            virtual void Visit( const DoWhileStatement & statement ) override;
            virtual void Visit( const BlockStatement & statement ) override;

        };
    }

#endif