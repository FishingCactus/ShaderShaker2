#ifndef ANNOTATION_PRINTER
    #define ANNOTATION_PRINTER

    #include "ast/const_visitor.h"
    #include <ostream>

    namespace AST
    {
        class AnnotationPrinter : public ConstVisitor
        {
        public:

            AnnotationPrinter( std::ostream & stream ) : m_Stream( stream ){}

            virtual void Visit( const Node & node ) override;
            AST_EmptyVisitOverride( TranslationUnit )
            AST_EmptyVisitOverride( VariableDeclaration )
            AST_EmptyVisitOverride( IntrinsicType )
            AST_EmptyVisitOverride( UserDefinedType )
            AST_EmptyVisitOverride( SamplerType )
            AST_EmptyVisitOverride( TypeModifier )
            AST_EmptyVisitOverride( StorageClass )
            AST_EmptyVisitOverride( VariableDeclarationBody )
            AST_EmptyVisitOverride( TextureDeclaration )
            AST_EmptyVisitOverride( SamplerDeclaration )
            AST_EmptyVisitOverride( SamplerBody )
            AST_EmptyVisitOverride( StructDefinition )
            AST_EmptyVisitOverride( FunctionDeclaration )
            AST_EmptyVisitOverride( ArgumentList )
            AST_EmptyVisitOverride( Argument )
            virtual void Visit( const Annotations & annotations ) override;
            virtual void Visit( const AnnotationEntry & annotation_entry ) override;

            // Expressions
            AST_EmptyVisitOverride( LiteralExpression )
            AST_EmptyVisitOverride( VariableExpression )
            AST_EmptyVisitOverride( UnaryOperationExpression )
            AST_EmptyVisitOverride( BinaryOperationExpression )
            AST_EmptyVisitOverride( CallExpression )
            AST_EmptyVisitOverride( ArgumentExpressionList )
            AST_EmptyVisitOverride( Swizzle )
            AST_EmptyVisitOverride( PostfixSuffixCall )
            AST_EmptyVisitOverride( PostfixSuffixVariable )
            AST_EmptyVisitOverride( ConstructorExpression )
            AST_EmptyVisitOverride( ConditionalExpression )
            AST_EmptyVisitOverride( LValueExpression )
            AST_EmptyVisitOverride( PreModifyExpression )
            AST_EmptyVisitOverride( PostModifyExpression )
            AST_EmptyVisitOverride( CastExpression )
            AST_EmptyVisitOverride( AssignmentExpression )
            AST_EmptyVisitOverride( PostfixExpression )

            // Statements

            AST_EmptyVisitOverride( ReturnStatement )
            AST_EmptyVisitOverride( BreakStatement )
            AST_EmptyVisitOverride( ContinueStatement )
            AST_EmptyVisitOverride( DiscardStatement )
            AST_EmptyVisitOverride( EmptyStatement )
            AST_EmptyVisitOverride( ExpressionStatement )
            AST_EmptyVisitOverride( IfStatement )
            AST_EmptyVisitOverride( WhileStatement )
            AST_EmptyVisitOverride( DoWhileStatement )
            AST_EmptyVisitOverride( BlockStatement )
            AST_EmptyVisitOverride( AssignmentStatement )
            AST_EmptyVisitOverride( VariableDeclarationStatement )

        private:

            std::ostream
                & m_Stream;
        };
    }
#endif