#include "node.h"

namespace AST
{
    #define CloneTable( _Type_, _member_ ) \
        { \
            std::vector< Base::ObjectRef<_Type_> >::const_iterator it, end; \
            \
            it = _member_.begin(); \
            end = _member_.end(); \
            for( ; it!=end; ++it ) \
            { \
                clone->_member_.push_back( (*it)->Clone() ); \
            } \
        }

    ReturnStatement * ReturnStatement::Clone() const
    {
        ReturnStatement * clone = new ReturnStatement;

        if( m_Expression )
        {
            clone->m_Expression = m_Expression->Clone();
        }

        return clone;
    }

    ExpressionStatement * ExpressionStatement::Clone() const
    {
        ExpressionStatement * clone = new ExpressionStatement;

        if( m_Expression )
        {
            clone->m_Expression = m_Expression->Clone();
        }

        return clone;
    }

    IfStatement * IfStatement::Clone() const
    {
        IfStatement * clone = new IfStatement;

        clone->m_Condition = m_Condition->Clone();
        clone->m_ThenStatement = m_ThenStatement->Clone();

        if( m_ElseStatement )
        {
            clone->m_ElseStatement = m_ElseStatement->Clone();
        }

        return clone;
    }

    WhileStatement * WhileStatement::Clone() const
    {
        WhileStatement * clone = new WhileStatement;

        clone->m_Condition = m_Condition->Clone();
        clone->m_Statement = m_Statement->Clone();

        return clone;
    }

    DoWhileStatement * DoWhileStatement::Clone() const
    {
        DoWhileStatement * clone = new DoWhileStatement;

        clone->m_Condition = m_Condition->Clone();
        clone->m_Statement = m_Statement->Clone();

        return clone;
    }

    ForStatement * ForStatement::Clone() const
    {
        ForStatement * clone = new ForStatement;

        clone->m_InitStatement = m_InitStatement->Clone();
        clone->m_Statement = m_Statement->Clone();
        clone->m_EqualityExpression = m_EqualityExpression->Clone();
        clone->m_ModifyExpression = m_ModifyExpression->Clone();

        return clone;
    }

    BlockStatement * BlockStatement::Clone() const
    {
        BlockStatement * clone = new BlockStatement;

        CloneTable( Statement, m_StatementTable );

        return clone;
    }

    VariableDeclarationStatement * VariableDeclarationStatement::Clone() const
    {
        VariableDeclarationStatement * clone = new VariableDeclarationStatement;

        clone->m_Type = m_Type->Clone();

        CloneTable( StorageClass, m_StorageClass )
        CloneTable( TypeModifier, m_TypeModifier )
        CloneTable( VariableDeclarationBody, m_BodyTable )

        return clone;
    }

    AssignmentStatement * AssignmentStatement::Clone() const
    {
        AssignmentStatement * clone = new AssignmentStatement;

        clone->m_Expression = m_Expression->Clone();

        return clone;
    }
}