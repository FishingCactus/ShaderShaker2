#include "legalizer.h"

#include <ast/node.h>

namespace AST
{
    void Legalizer::Visit( Node & /*node*/ )
    {
        assert( !"Unsupported node type, implement in base class" );
    }

    void Legalizer::Visit( VariableDeclaration & node )
    {
        m_CurrentVariableDeclaration = &node;

        TreeTraverser::Visit( node );

        m_CurrentVariableDeclaration = nullptr;
    }

    void Legalizer::Visit( LiteralExpression & node )
    {
        if ( m_CurrentVariableDeclaration )
        {
            const std::string
                & variable_type = m_CurrentVariableDeclaration->m_Type->m_Name;
            
            if ( variable_type == "float"
                 && node.m_Type == LiteralExpression::Int
                 )
            {
                node.m_Type = LiteralExpression::Float;

                if ( node.m_Value.find_last_of( '.' ) == std::string::npos )
                {
                    node.m_Value.append( ".0f" );
                }
                else if ( node.m_Value.find_last_of( 'f' ) == std::string::npos )
                {
                    node.m_Value.append( "f" );
                }
            }
        }

        TreeTraverser::Visit( node );
    }
}