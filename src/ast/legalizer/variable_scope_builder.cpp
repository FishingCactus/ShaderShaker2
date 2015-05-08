#include "variable_scope_builder.h"

#include <ast/node.h>

namespace AST
{
    VariableScopeBuilder::VariableScopeBuilder( ScopeBuilder::ScopeData & scope_data )
        : m_ScopeData(scope_data), m_CancelNextBlockScope( false )
    {
        m_ScopeStack.push( &scope_data.m_GlobalScope );
    }

    void VariableScopeBuilder::Visit( const Node & /*node*/ )
    {
        assert( !"Unsupported node type, implement in base class" );
    }

    void VariableScopeBuilder::Visit( const TranslationUnit & node )
    {
        ConstTreeTraverser::Visit( node );
    }

    void VariableScopeBuilder::Visit( const VariableDeclaration & node )
    {
        m_CurrentVariableDeclarationType = node.m_Type->m_Name;

        ConstTreeTraverser::Visit( node );

        m_CurrentVariableDeclarationType = "";
    }

    void VariableScopeBuilder::Visit( const VariableDeclarationStatement & node )
    {
        m_CurrentVariableDeclarationType = node.m_Type->m_Name;

        ConstTreeTraverser::Visit( node );

        m_CurrentVariableDeclarationType = "";
    }

    void VariableScopeBuilder::Visit( const VariableDeclarationBody & node )
    {
        assert( !m_CurrentVariableDeclarationType.empty() );

        ScopeBuilder::Variable
            * variable = AddVariableToCurrentScope();

        variable->m_Type = m_CurrentVariableDeclarationType;
        variable->m_Name = node.m_Name;

        ConstTreeTraverser::Visit( node );
    }

    void VariableScopeBuilder::Visit( const FunctionDeclaration & node )
    {
        NewScopeHelper
            creation_helper( *this, node, node.m_Name, node.m_Type ? node.m_Type->m_Name : "void" );

        ConstTreeTraverser::Visit( node );
    }

    void VariableScopeBuilder::Visit( const Argument & node )
    {
        ScopeBuilder::Variable
            * argument_variable = AddVariableToCurrentScope();

        argument_variable->m_Name = node.m_Name;
        argument_variable->m_Type = node.m_Type->m_Name;

        ConstTreeTraverser::Visit( node );
    }

    void VariableScopeBuilder::Visit( const StructDefinition & node )
    {
        NewScopeHelper
            creation_helper( *this, node, node.m_Name, "" );

        for ( auto member : node.m_MemberTable )
        {
            ScopeBuilder::Variable
                * argument_variable = AddVariableToCurrentScope();

            argument_variable->m_Name = member.m_Name;
            argument_variable->m_Type = member.m_Type->m_Name;
        }

        ConstTreeTraverser::Visit( node );
    }

    void VariableScopeBuilder::Visit( const IfStatement & node )
    {
        node.m_Condition->Visit( *this );

        m_CurrentScopeBlockName = "if";
        node.m_ThenStatement->Visit( *this );

        if ( node.m_ElseStatement )
        {
            m_CurrentScopeBlockName = "else";
            node.m_ElseStatement->Visit( *this );
        }

        m_CurrentScopeBlockName = "";
    }

    void VariableScopeBuilder::Visit( const ForStatement & node )
    {
        NewScopeHelper
            creation_helper( *this, node, "for", "block" );

        if ( node.m_InitStatement )
        {
            node.m_InitStatement->Visit( *this );
        }
        if ( node.m_EqualityExpression )
        {
            node.m_EqualityExpression->Visit( *this );
        }
        if ( node.m_ModifyExpression )
        {
            node.m_ModifyExpression->Visit( *this );
        }

        m_CancelNextBlockScope = true;
        node.m_Statement->Visit( *this );
    }

    void VariableScopeBuilder::Visit( const WhileStatement & node )
    {
        NewScopeHelper
            creation_helper( *this, node, "while", "block" );

        // Don't visit condition, as we don't support constructions like while ( float f = 1.0f ) {}

        m_CancelNextBlockScope = true;
        node.m_Statement->Visit( *this );
    }

    void VariableScopeBuilder::Visit( const DoWhileStatement & node )
    {
        NewScopeHelper
            creation_helper( *this, node, "do", "block" );

        m_CancelNextBlockScope = true;

        node.m_Statement->Visit( *this );
    }

    void VariableScopeBuilder::Visit( const BlockStatement & node )
    {
        if ( !m_CancelNextBlockScope )
        {
            NewScopeHelper
                creation_helper( *this, node, m_CurrentScopeBlockName, "block" );

            ConstTreeTraverser::Visit( node );
        }
        else
        {
            m_CancelNextBlockScope = false;
            ConstTreeTraverser::Visit( node );
        }
    }

    ScopeBuilder::Variable * VariableScopeBuilder::AddVariableToCurrentScope()
    {
        ScopeBuilder::Scope
            & current_scope = GetCurrentScope();
        ScopeBuilder::Variable
            * variable = new ScopeBuilder::Variable( current_scope );

        current_scope.m_Variables.push_back( variable );

        return variable;
    }

    ScopeBuilder::Scope & VariableScopeBuilder::GetCurrentScope()
    {
        return *m_ScopeStack.top();
    }
}
