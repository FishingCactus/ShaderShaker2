#ifndef GRAPH_VALIDATOR_H
    #define GRAPH_VALIDATOR_H

    #include <string>
    #include <set>
    #include <map>
    #include <base/error_handler_interface.h>

    namespace Generation
    {
        class GraphNode;
        class GraphValidator
        {
        public:
            GraphValidator( Base::ErrorHandlerInterface & error_handler );

            void Visit( const GraphNode & node );

            bool HasErrors() const { return m_GraphHasErrors; }

        private:

            Base::ErrorHandlerInterface::Ref
                m_ErrorHandler;
            std::set<const GraphNode*>
                m_VisitedNodeSet;
            std::map<std::string, std::string>
                m_SemanticToTypeMap;
            bool
                m_GraphHasErrors;
        };

    }

#endif