#ifndef GRAPH_H
    #define GRAPH_H

    #include <vector>
    #include <string>
    #include <map>
    #include <set>
    #include "graph_node.h"

    namespace Generation
    {
        class Graph : public Base::Object
        {
        public:

            typedef Base::ObjectRef<Graph>
                Ref;

            void Initialize( const std::set<std::string> & semantic_set );
            bool AddNode( GraphNode & node );

            bool HasGeneratedSemantic( const std::string & semantic ) const;
            bool UseGeneratedSemantic(
                GraphNode & node,
                const std::string & semantic
                );

            template< typename Visitor >
            void VisitDepthFirst( Visitor & visitor ) const
            {
                for( std::vector<GraphNode::Ref>::const_iterator it = m_RootNodeTable.begin(), end = m_RootNodeTable.end();
                    it != end;
                    ++it )
                {
                    (*it)->VisitDepthFirst( visitor );
                }
            }

        private:

            std::vector<GraphNode::Ref>
                m_RootNodeTable;
            std::multimap<std::string, GraphNode::Ref >
                m_NodeRequiringSemanticMap;
            std::map<std::string, GraphNode::Ref >
                m_NodeToLastOutputSemanticMap;

        };
    }

#endif