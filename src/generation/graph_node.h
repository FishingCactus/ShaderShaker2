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

            GraphNode( FunctionDefinition & definition );

		private:

            FunctionDefinition::Ref
                m_FunctionDefinition;
			std::vector<GraphNode*>
				m_Parent;
			std::vector<Base::ObjectRef<GraphNode>>
				m_Children;
		};
	}

#endif