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