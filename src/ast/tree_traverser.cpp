#include "tree_traverser.h"

#include <cassert>

#include <ast/node.h>

namespace AST
{

    void TreeTraverser::Visit( TranslationUnit & translation_unit )
    {
        VisitTable( *this, translation_unit.m_GlobalDeclarationTable );
        VisitTable( *this, translation_unit.m_TechniqueTable );
    }

    void TreeTraverser::Visit( VariableDeclaration & variable_declaration )
    {
        VisitTable( *this, variable_declaration.m_StorageClass );
        VisitTable( *this, variable_declaration.m_TypeModifier );
        VisitTable( *this, variable_declaration.m_BodyTable );
    }

    void TreeTraverser::Visit( IntrinsicType & /*type*/ )
    {
    }

    void TreeTraverser::Visit( UserDefinedType & /*type*/ )
    {
    }

    void TreeTraverser::Visit( SamplerType & /*type*/ )
    {
    }

    void TreeTraverser::Visit( TypeModifier & /*modifier*/ )
    {
    }

    void TreeTraverser::Visit( StorageClass & /*storage_class*/ )
    {
    }

    void TreeTraverser::Visit( ArgumentList & list )
    {
        VisitTable( *this, list.m_ArgumentTable );
    }

    void TreeTraverser::Visit( LiteralExpression & /*expression*/ )
    {
    }

    void TreeTraverser::Visit( VariableExpression & expression )
    {
        if ( expression.m_SubscriptExpression )
        {
            expression.m_SubscriptExpression->Visit( *this );
        }
    }

    void TreeTraverser::Visit( UnaryOperationExpression & expression )
    {
        expression.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( BinaryOperationExpression & expression )
    {
        expression.m_LeftExpression->Visit( *this );
        expression.m_RightExpression->Visit( *this );
    }

    void TreeTraverser::Visit( CallExpression & expression )
    {
        if ( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }
    }

    void TreeTraverser::Visit( ArgumentExpressionList & list )
    {
        VisitTable( *this, list.m_ExpressionList );
    }

    void TreeTraverser::Visit( Swizzle & /*swizzle*/ )
    {
    }

    void TreeTraverser::Visit( PostfixSuffixCall & postfix_suffix )
    {
        postfix_suffix.m_CallExpression->Visit( *this );

        if ( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void TreeTraverser::Visit( PostfixSuffixVariable & postfix_suffix )
    {
        postfix_suffix.m_VariableExpression->Visit( *this );

        if ( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void TreeTraverser::Visit( ConstructorExpression & expression )
    {
        expression.m_Type->Visit( *this );

        if ( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }
    }

    void TreeTraverser::Visit( ConditionalExpression & expression )
    {
        expression.m_Condition->Visit( *this );
        expression.m_IfTrue->Visit( *this );
        expression.m_IfFalse->Visit( *this );
    }

    void TreeTraverser::Visit( LValueExpression & expression )
    {
        expression.m_VariableExpression->Visit( *this );

        if ( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void TreeTraverser::Visit( PreModifyExpression & expression )
    {
        expression.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( PostModifyExpression & expression )
    {
        expression.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( CastExpression & expression )
    {
        expression.m_Type->Visit( *this );
        expression.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( AssignmentExpression & expression )
    {
        expression.m_LValueExpression->Visit( *this );
        expression.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( PostfixExpression & expression )
    {
        expression.m_Expression->Visit( *this );

        if ( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void TreeTraverser::Visit( ReturnStatement & statement )
    {
        if ( statement.m_Expression )
        {
            statement.m_Expression->Visit( *this );
        }
    }

    void TreeTraverser::Visit( BreakStatement & /*statement*/ )
    {
    }

    void TreeTraverser::Visit( ContinueStatement & /*statement*/ )
    {
    }

    void TreeTraverser::Visit( EmptyStatement & /*statement*/ )
    {
    }

    void TreeTraverser::Visit( ExpressionStatement & statement )
    {
        statement.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( IfStatement & statement )
    {
        statement.m_Condition->Visit( *this );
        statement.m_ThenStatement->Visit( *this );

        if ( statement.m_ElseStatement )
        {
            statement.m_ElseStatement->Visit( *this );
        }
    }

    void TreeTraverser::Visit( WhileStatement & statement )
    {
        statement.m_Condition->Visit( *this );
        statement.m_Statement->Visit( *this );
    }

    void TreeTraverser::Visit( DoWhileStatement & statement )
    {
        statement.m_Statement->Visit( *this );
        statement.m_Condition->Visit( *this );
    }

    void TreeTraverser::Visit( ForStatement & statement )
    {
        if ( statement.m_InitStatement )
        {
            statement.m_InitStatement->Visit( *this );
        }
        if ( statement.m_EqualityExpression )
        {
            statement.m_EqualityExpression->Visit( *this );
        }
        if ( statement.m_ModifyExpression )
        {
            statement.m_ModifyExpression->Visit( *this );
        }

        statement.m_Statement->Visit( *this );
    }

    void TreeTraverser::Visit( BlockStatement & statement )
    {
        VisitTable( *this, statement.m_StatementTable );
    }

    void TreeTraverser::Visit( AssignmentStatement & statement )
    {
        statement.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( VariableDeclarationStatement & statement )
    {
        VisitTable( *this, statement.m_StorageClass );
        VisitTable( *this, statement.m_TypeModifier );
        VisitTable( *this, statement.m_BodyTable );
    }

    void TreeTraverser::Visit( VariableDeclarationBody & body )
    {
        if ( body.m_Annotations )
        {
            body.m_Annotations->Visit( *this );
        }

        if ( body.m_InitialValue )
        {
            body.m_InitialValue->Visit( *this );
        }
    }

    void TreeTraverser::Visit( InitialValue & initial_value )
    {
        if ( initial_value.m_Vector )
        {
            VisitTable( *this, initial_value.m_ExpressionTable );
        }
        else
        {
            assert( initial_value.m_ExpressionTable.size() == 1 );
            initial_value.m_ExpressionTable[ 0 ]->Visit( *this );
        }
    }

    void TreeTraverser::Visit( Annotations & annotations )
    {
        VisitTable( *this, annotations.m_AnnotationTable );
    }

    void TreeTraverser::Visit( AnnotationEntry & /*annotation_entry*/ )
    {
    }

    void TreeTraverser::Visit( TextureDeclaration & declaration )
    {
        if ( declaration.m_Annotations )
        {
            declaration.m_Annotations->Visit( *this );
        }
    }

    void TreeTraverser::Visit( SamplerDeclaration & declaration )
    {
        VisitTable( *this, declaration.m_BodyTable );
    }

    void TreeTraverser::Visit( SamplerBody & /*body*/ )
    {
    }

    void TreeTraverser::Visit( StructDefinition & definition )
    {
        std::vector< StructDefinition::Member >::iterator it, end;
        it = definition.m_MemberTable.begin();
        end = definition.m_MemberTable.end();

        for ( ; it != end; ++it )
        {
            StructDefinition::Member
                & member = ( *it );

            member.m_Type->Visit( *this );
        }
    }

    void TreeTraverser::Visit( FunctionDeclaration & declaration )
    {
        VisitTable( *this, declaration.m_StorageClassTable );

        if ( declaration.m_Type )
        {
            declaration.m_Type->Visit( *this );
        }

        if ( declaration.m_ArgumentList )
        {
            declaration.m_ArgumentList->Visit( *this );
        }

        VisitTable( *this, declaration.m_StatementTable );
    }

    void TreeTraverser::Visit( Argument & argument )
    {
        if ( argument.m_TypeModifier )
        {
            argument.m_TypeModifier->Visit( *this );
        }

        if ( argument.m_InitialValue )
        {
            argument.m_InitialValue->Visit( *this );
        }
    }

    void TreeTraverser::Visit( DiscardStatement & /*statement*/ )
    {
    }
}