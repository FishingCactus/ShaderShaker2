#ifndef GRAPH_NODE_H
    #define GRAPH_NODE_H

    #include <vector>
    #include <base/object_ref.h>
    #include <base/object.h>

    #include "function_definition.h"

    namespace Generation
    {
        class GraphNode : public Base::Object
        {

        public:

            typedef Base::ObjectRef<GraphNode>
                Ref;

            GraphNode();
            GraphNode( FunctionDefinition & definition );

            void AddParent( GraphNode & parent );
            FunctionDefinition & GetFunctionDefinition(){ return *m_FunctionDefinition; }

        private:

            FunctionDefinition::Ref
                m_FunctionDefinition;
            std::vector<GraphNode*>
                m_Parents;
            std::vector<GraphNode::Ref>
                m_Children;
        };
    }

#endif