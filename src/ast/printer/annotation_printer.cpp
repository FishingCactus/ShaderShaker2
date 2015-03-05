#include "annotation_printer.h"

#include <cassert>
#include <utils/indentation.h>
#include <ast/node.h>
#include <algorithm>

namespace AST
{
    void AnnotationPrinter::Visit( const Node & node )
    {
        assert( !"Unsupported node type, implement in base class" );
    }

    void AnnotationPrinter::Visit( const Annotations & annotations )
    {
        m_Stream
            << "\"annotations\":{";

        std::copy( annotations.m_AnnotationTable.begin(), annotations.m_AnnotationTable.end(), std::ostream_iterator< AnnotationEntry >( m_Stream, "," ) );

        m_Stream << "}";
    }

    void AnnotationPrinter::Visit( const AnnotationEntry & annotation_entry )
    {
        if ( annotation_entry.m_Name.empty()
             || annotation_entry.m_Type.empty()
             || annotation_entry.m_Value.empty()
             )
        {
            return;
        }

        m_Stream
            << "\"" << annotation_entry.m_Name << "\":"
            << "{"
                << "\"type\":\"" << annotation_entry.m_Type << "\","
                << "\"value\":\"" << annotation_entry.m_Value << "\""
            << "}";
    }
}