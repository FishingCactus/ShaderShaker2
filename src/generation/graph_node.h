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

            const std::vector<GraphNode::Ref> & GetChildren() const
            {
                return m_Children;
            }

            void AddParent( GraphNode & parent );
            FunctionDefinition & GetFunctionDefinition(){ return *m_FunctionDefinition; }
            const FunctionDefinition & GetFunctionDefinition()const{ return *m_FunctionDefinition; }
            bool HasFunctionDefinition() const{ return m_FunctionDefinition; }

            template< typename Visitor >
            void VisitDepthFirst( Visitor & visitor ) const
            {
                for( std::vector<GraphNode::Ref>::const_iterator it = m_Children.begin(), end = m_Children.end();
                    it != end;
                    ++it )
                {
                    (*it)->VisitDepthFirst( visitor );
                }

                visitor.Visit( *this );
            }

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