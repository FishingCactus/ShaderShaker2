#ifndef HLSL_PRINTER
    #define HLSL_PRINTER

    #include "ast/visitor.h"
    #include <ostream>

    namespace AST
    {

        class HLSLPrinter : public Visitor
        {
        public:

            HLSLPrinter( std::ostream & stream ) : m_Stream( stream ){}

            virtual void Visit( Node & node ) override;
            virtual void Visit( TranslationUnit & translation_unit ) override;
            virtual void Visit( VariableDeclaration & variable_declaration ) override;
            virtual void Visit( IntrinsicType & type ) override;
            virtual void Visit( UserDefinedType & type ) override;
            virtual void Visit( SamplerType & type ) override;
            virtual void Visit( TypeModifier & modifier ) override;
            virtual void Visit( StorageClass & storage_class ) override;
            virtual void Visit( VariableDeclarationBody & body ) override;
            virtual void Visit( TextureDeclaration & declaration ) override;
            virtual void Visit( SamplerDeclaration & declaration ) override;
            virtual void Visit( SamplerBody & body ) override;
            virtual void Visit( StructDefinition & definition ) override;

            virtual void Visit( LiteralExpression & expression ) override;
            virtual void Visit( VariableExpression & expression ) override;
            virtual void Visit( UnaryOperationExpression & expression ) override;
            virtual void Visit( BinaryOperationExpression & expression ) override;
            virtual void Visit( CallExpression & expression ) override;
            virtual void Visit( ArgumentExpressionList & list ) override;
            virtual void Visit( Swizzle & swizzle ) override;
            virtual void Visit( PostfixSuffixCall & postfix_suffix ) override;
            virtual void Visit( PostfixSuffixVariable & postfix_suffix ) override;
            virtual void Visit( ConstructorExpression & expression ) override;
            virtual void Visit( ConditionalExpression & expression ) override;
            virtual void Visit( LValueExpression & expression ) override;
            virtual void Visit( PreModifyExpression & expression ) override;
            virtual void Visit( PostModifyExpression & expression ) override;

            virtual void Visit( ReturnStatement & statement ) override;
            virtual void Visit( BreakStatement & statement ) override;
            virtual void Visit( ContinueStatement & statement ) override;
            virtual void Visit( DiscardStatement & statement ) override;
            virtual void Visit( EmptyStatement & statement ) override;
            virtual void Visit( ExpressionStatement & statement ) override;
            virtual void Visit( IfStatement & statement ) override;
            virtual void Visit( WhileStatement & statement ) override;
            virtual void Visit( DoWhileStatement & statement ) override;
            virtual void Visit( BlockStatement & statement ) override;

        private:

            std::ostream
                & m_Stream;

        };
    }


#endif
