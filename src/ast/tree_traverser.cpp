#include "tree_traverser.h"

#include <cassert>

#include <ast/node.h>

namespace AST
{

    void TreeTraverser::Visit( const TranslationUnit & translation_unit )
    {
        AST::VisitTable( *this, translation_unit.m_GlobalDeclarationTable );
        AST::VisitTable( *this, translation_unit.m_TechniqueTable );
    }

    void TreeTraverser::Visit( const VariableDeclaration & variable_declaration )
    {
        VisitTable( *this, variable_declaration.m_StorageClass );
        VisitTable( *this, variable_declaration.m_TypeModifier );
        VisitTable( *this, variable_declaration.m_BodyTable );
    }

    void TreeTraverser::Visit( const IntrinsicType & /*type*/ )
    {
    }

    void TreeTraverser::Visit( const UserDefinedType & /*type*/ )
    {
    }

    void TreeTraverser::Visit( const SamplerType & /*type*/ )
    {
    }

    void TreeTraverser::Visit( const TypeModifier & /*modifier*/ )
    {
    }

    void TreeTraverser::Visit( const StorageClass & /*storage_class*/ )
    {
    }

    void TreeTraverser::Visit( const ArgumentList & list )
    {
        VisitTable( *this, list.m_ArgumentTable );
    }

    void TreeTraverser::Visit( const LiteralExpression & /*expression*/ )
    {
    }

    void TreeTraverser::Visit( const VariableExpression & expression )
    {
        if ( expression.m_SubscriptExpression )
        {
            expression.m_SubscriptExpression->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const UnaryOperationExpression & expression )
    {
        expression.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( const BinaryOperationExpression & expression )
    {
        expression.m_LeftExpression->Visit( *this );
        expression.m_RightExpression->Visit( *this );
    }

    void TreeTraverser::Visit( const CallExpression & expression )
    {
        if ( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const ArgumentExpressionList & list )
    {
        VisitTable( *this, list.m_ExpressionList );
    }

    void TreeTraverser::Visit( const Swizzle & /*swizzle*/ )
    {
    }

    void TreeTraverser::Visit( const PostfixSuffixCall & postfix_suffix )
    {
        postfix_suffix.m_CallExpression->Visit( *this );

        if ( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const PostfixSuffixVariable & postfix_suffix )
    {
        postfix_suffix.m_VariableExpression->Visit( *this );

        if ( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const ConstructorExpression & expression )
    {
        expression.m_Type->Visit( *this );

        if ( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const ConditionalExpression & expression )
    {
        expression.m_Condition->Visit( *this );
        expression.m_IfTrue->Visit( *this );
        expression.m_IfFalse->Visit( *this );
    }

    void TreeTraverser::Visit( const LValueExpression & expression )
    {
        expression.m_VariableExpression->Visit( *this );

        if ( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const PreModifyExpression & expression )
    {
        expression.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( const PostModifyExpression & expression )
    {
        expression.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( const CastExpression & expression )
    {
        expression.m_Type->Visit( *this );
        expression.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( const AssignmentExpression & expression )
    {
        expression.m_LValueExpression->Visit( *this );
        expression.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( const PostfixExpression & expression )
    {
        expression.m_Expression->Visit( *this );

        if ( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const ReturnStatement & statement )
    {
        if ( statement.m_Expression )
        {
            statement.m_Expression->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const BreakStatement & /*statement*/ )
    {
    }

    void TreeTraverser::Visit( const ContinueStatement & /*statement*/ )
    {
    }

    void TreeTraverser::Visit( const EmptyStatement & /*statement*/ )
    {
    }

    void TreeTraverser::Visit( const ExpressionStatement & statement )
    {
        statement.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( const IfStatement & statement )
    {
        statement.m_Condition->Visit( *this );
        statement.m_ThenStatement->Visit( *this );

        if ( statement.m_ElseStatement )
        {
            statement.m_ElseStatement->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const WhileStatement & statement )
    {
        statement.m_Condition->Visit( *this );
        statement.m_Statement->Visit( *this );
    }

    void TreeTraverser::Visit( const DoWhileStatement & statement )
    {
        statement.m_Statement->Visit( *this );
        statement.m_Condition->Visit( *this );
    }

    void TreeTraverser::Visit( const BlockStatement & statement )
    {
        VisitTable( *this, statement.m_StatementTable );
    }

    void TreeTraverser::Visit( const AssignmentStatement & statement )
    {
        statement.m_Expression->Visit( *this );
    }

    void TreeTraverser::Visit( const VariableDeclarationStatement & statement )
    {
        VisitTable( *this, statement.m_StorageClass );
        VisitTable( *this, statement.m_TypeModifier );
        VisitTable( *this, statement.m_BodyTable );
    }

    void TreeTraverser::Visit( const VariableDeclarationBody & body )
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

    void TreeTraverser::Visit( const InitialValue & initial_value )
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

    void TreeTraverser::Visit( const Annotations & annotations )
    {
        VisitTable( *this, annotations.m_AnnotationTable );
    }

    void TreeTraverser::Visit( const AnnotationEntry & /*annotation_entry*/ )
    {
    }

    void TreeTraverser::Visit( const TextureDeclaration & declaration )
    {
        if ( declaration.m_Annotations )
        {
            declaration.m_Annotations->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const SamplerDeclaration & declaration )
    {
        VisitTable( *this, declaration.m_BodyTable );
    }

    void TreeTraverser::Visit( const SamplerBody & /*body*/ )
    {
    }

    void TreeTraverser::Visit( const StructDefinition & definition )
    {
        std::vector< StructDefinition::Member >::const_iterator it, end;
        it = definition.m_MemberTable.cbegin();
        end = definition.m_MemberTable.cend();

        for ( ; it != end; ++it )
        {
            const StructDefinition::Member
                & member = ( *it );

            member.m_Type->Visit( *this );
        }
    }

    void TreeTraverser::Visit( const FunctionDeclaration & declaration )
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

    void TreeTraverser::Visit( const Argument & argument )
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

    void TreeTraverser::Visit( const DiscardStatement & /*statement*/ )
    {
    }
}