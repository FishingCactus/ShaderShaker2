#include "graph.h"


namespace Generation
{

    void Graph::Initialize( const std::set<std::string> & semantic_set )
    {
        std::set<std::string>::const_iterator it, end;
        it = semantic_set.begin();
        end = semantic_set.end();

        for(; it != end; ++it)
        {
            GraphNode::Ref node = new GraphNode;

            m_RootNodeTable.push_back( node );
            m_NodeRequiringSemanticMap.insert( std::make_pair( *it, node ) );
        }

    }

    bool Graph::AddNode( GraphNode & node )
    {
        std::set<std::string>
            output_semantic,
            input_semantic;

        // :TODO: Search for cycle when resolving semantic

        node.GetFunctionDefinition().GetAllOutSemanticSet( output_semantic );
        node.GetFunctionDefinition().GetAllInSemanticSet( input_semantic );

        std::set<std::string>::const_iterator it, end;

        it = output_semantic.begin();
        end = output_semantic.end();

        for( ;it!=end; ++it )
        {
            std::string semantic = (*it);
            typedef std::multimap<std::string, Base::ObjectRef<GraphNode> >::iterator node_iterator;

            std::pair<node_iterator, node_iterator> range;

            range = m_NodeRequiringSemanticMap.equal_range( semantic );
            node_iterator it;

            for( it = range.first; it != range.second; ++it )
            {
                node.AddParent( *(*it).second );
            }

            m_NodeRequiringSemanticMap.erase( range.first, range.second );

            m_NodeToLastOutputSemanticMap[ semantic ] = &node;
        }

        it = input_semantic.begin();
        end = input_semantic.end();

        for( ;it!=end; ++it )
        {
            m_NodeRequiringSemanticMap.insert( std::make_pair( *it, &node ) );
        }

        return true;
    }

    bool Graph::HasGeneratedSemantic(
        const std::string & semantic
        ) const
    {
        return m_NodeToLastOutputSemanticMap.find( semantic ) != m_NodeToLastOutputSemanticMap.end();
    }

    bool Graph::UseGeneratedSemantic(
        GraphNode & node,
        const std::string & semantic
        )
    {
        std::map<std::string, GraphNode::Ref >::iterator
            generating_node;

        generating_node = m_NodeToLastOutputSemanticMap.find( semantic );

        assert( generating_node != m_NodeToLastOutputSemanticMap.end() );

        return (*generating_node).second->AddParent( node );
    }
}