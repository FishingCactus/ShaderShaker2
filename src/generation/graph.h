#ifndef GRAPH_H
	#define GRAPH_H

    #include <vector>
    #include <base/object_ref.h>
    #include <base/object.h>
    #include "graph_node.h"

	namespace Generation
	{
		class Graph : public Base::Object
		{
		public:

			typedef Base::ObjectRef<Graph>
				Ref;
            
            bool AddNode( GraphNode & node );

		private:

			std::vector<Base::ObjectRef<GraphNode> >
				m_RootNodeTable;
		};
	}

#endif