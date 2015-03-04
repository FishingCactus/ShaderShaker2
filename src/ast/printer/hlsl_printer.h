#ifndef HLSL_PRINTER
    #define HLSL_PRINTER

    #include "ast/const_visitor.h"
    #include <ostream>

    namespace AST
    {

        class HLSLPrinter : public ConstVisitor
        {
        public:

            HLSLPrinter( std::ostream & stream ) : m_Stream( stream ){}

            virtual void Visit( const Node & node ) override;
            virtual void Visit( const TranslationUnit & translation_unit ) override;
            virtual void Visit( const VariableDeclaration & variable_declaration ) override;
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
            virtual void Visit( const FunctionDeclaration & declaration ) override;
            virtual void Visit( const ArgumentList & list ) override;
            virtual void Visit( const Argument & argument ) override;

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
            virtual void Visit( const PostfixExpression & expression ) override;

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
            virtual void Visit( const AssignmentStatement & statement ) override;
            virtual void Visit( const VariableDeclarationStatement & statement ) override;

        private:

            std::ostream
                & m_Stream;

        };
    }
#endif
