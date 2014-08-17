#include "graph_node.h"

#include <queue>

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

    bool GraphNode::AddParent( GraphNode & parent )
    {
        if( parent.NodeExistsInParents( *this ) )
        {
            return false;
        }

        m_Parents.push_back( &parent );
        parent.m_Children.push_back( this );

        return true;
    }

    bool GraphNode::NodeExistsInParents(
        const GraphNode & node
        ) const
    {
        std::queue<const GraphNode*>
            node_to_visit;

        node_to_visit.push( this );

        while( !node_to_visit.empty() )
        {
            const GraphNode * current = node_to_visit.front();

            node_to_visit.pop();

            if( &node == current )
            {
                return true;
            }

            std::vector<GraphNode*>::const_iterator it, end;
            it = current->m_Parents.begin();
            end = current->m_Parents.end();

            for( ;it != end; ++it )
            {
                node_to_visit.push( *it );
            }
        }

        return false;
    }
}