#include "graph_printer.h"

#include <generation/graph_node.h>
namespace Generation
{

    GraphPrinter::GraphPrinter(
        std::ostream & output_stream
        ) :
        m_OutputStream( output_stream )
    {

    }

    void GraphPrinter::Initialize()
    {
        m_OutputStream << "digraph G {" << std::endl;
    }

    void GraphPrinter::Visit(
        const GraphNode & node
        )
    {
        if( m_VisitedNodeSet.find( &node ) != m_VisitedNodeSet.end() )
        {
            return;
        }

        m_VisitedNodeSet.insert( &node );

        if( node.HasFunctionDefinition() )
        {
            m_OutputStream << "node_" << &node
                << " [label=\"" << node.GetFunctionDefinition().GetName()
                << "\"];" << std::endl;
        }

        for( auto child : node.GetChildren() )
        {
            m_OutputStream << "node_" << &node << "->node_" << &*child << ";" << std::endl;
        }
    }

    void GraphPrinter::Finalize()
    {
        m_OutputStream << " }";
    }
}
