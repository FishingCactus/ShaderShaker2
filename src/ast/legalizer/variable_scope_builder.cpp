#include "variable_scope_builder.h"

#include <ast/node.h>

namespace AST
{
    VariableScopeBuilder::VariableScopeBuilder() 
        : m_CurrentVariable( nullptr ) 
    {
        m_CurrentScope = &m_GlobalScope;
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
            creation_helper( *this, node.m_Name, node.m_Type ? node.m_Type->m_Name : "void" );

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
        return *m_CurrentScope;
    }
}
