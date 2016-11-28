#include "annotation_printer.h"

#include <cassert>
#include <utils/indentation.h>
#include <ast/node.h>
#include <algorithm>
#include "ast/visitor_item_separator.h"

namespace AST
{
    void AnnotationPrinter::Visit( const Node & /*node*/ )
    {
        assert( !"Unsupported node type, implement in base class" );
    }

    void AnnotationPrinter::Visit( const Annotations & annotations )
    {
        VisitorStreamArrayItemSeparator< Annotations::AnnotationTableType >
            separator( m_Stream );

        m_Stream
            << "{";

        VisitTable( *this, annotations.m_AnnotationTable, separator );

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

    void AnnotationPrinter::Visit( const VariableDeclarationBody & body )
    {
        if ( body.m_Semantic != "" )
        {
            m_Stream << "\"" << body.m_Semantic << "\":";
        }
        TreeTraverser::Visit(body);
    }

}