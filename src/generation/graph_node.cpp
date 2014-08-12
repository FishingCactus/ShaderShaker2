#include "graph_node.h"

namespace Generation
{
    GraphNode::GraphNode()
    {

    }

    GraphNode::GraphNode(
        FunctionDefinition & definition
        ) :
        m_FunctionDefinition( &definition )
    {

    }

        void GraphNode::AddParent( GraphNode & parent )
        {
            m_Parents.push_back( &parent );
            parent.m_Children.push_back( this );
        }

}