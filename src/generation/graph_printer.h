#ifndef GRAPH_PRINTER_H
    #define GRAPH_PRINTER_H

    #include <ostream>
    #include <set>

    namespace Generation
    {
        class GraphNode;

        struct GraphPrinter
        {
            GraphPrinter( std::ostream & output_stream );
            void Initialize();
            void Visit( const GraphNode & node );
            void Finalize();

            std::ostream
                & m_OutputStream;
            std::set< const GraphNode * >
                m_VisitedNodeSet;
        };

    }

#endif