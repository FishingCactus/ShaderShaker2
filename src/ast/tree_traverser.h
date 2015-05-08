#pragma once

#include "ast/visitor.h"

namespace AST
{

    class TreeTraverser : public Visitor
    {
    public:

        virtual void Visit( TranslationUnit & translation_unit ) override;
        virtual void Visit( VariableDeclaration & variable_declaration ) override;
        virtual void Visit( IntrinsicType & type ) override;
        virtual void Visit( UserDefinedType & type ) override;
        virtual void Visit( SamplerType & type ) override;
        virtual void Visit( TypeModifier & modifier ) override;
        virtual void Visit( StorageClass & storage_class ) override;
        virtual void Visit( ArgumentList & list ) override;

        virtual void Visit( VariableDeclarationBody & body ) override;
        virtual void Visit( InitialValue & initial_value ) override;
        virtual void Visit( Annotations & annotations ) override;
        virtual void Visit( AnnotationEntry & annotation_entry ) override;
        virtual void Visit( TextureDeclaration & declaration ) override;
        virtual void Visit( SamplerDeclaration & declaration ) override;
        virtual void Visit( SamplerBody & body ) override;
        virtual void Visit( StructDefinition & definition ) override;
        virtual void Visit( FunctionDeclaration & declaration ) override;
        virtual void Visit( Argument & argument ) override;
        virtual void Visit( DiscardStatement & statement ) override;

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
        virtual void Visit( CastExpression & expression ) override;
        virtual void Visit( AssignmentExpression & expression ) override;
        virtual void Visit( PostfixExpression & expression ) override;

        virtual void Visit( ReturnStatement & statement ) override;
        virtual void Visit( BreakStatement & statement ) override;
        virtual void Visit( ContinueStatement & statement ) override;
        virtual void Visit( EmptyStatement & statement ) override;
        virtual void Visit( ExpressionStatement & statement ) override;
        virtual void Visit( IfStatement & statement ) override;
        virtual void Visit( WhileStatement & statement ) override;
        virtual void Visit( DoWhileStatement & statement ) override;
        virtual void Visit( ForStatement & statement ) override;
        virtual void Visit( BlockStatement & statement ) override;
        virtual void Visit( AssignmentStatement & statement ) override;
        virtual void Visit( VariableDeclarationStatement & statement ) override;

    protected:

        TreeTraverser & operator =( const TreeTraverser & other );
    };
}