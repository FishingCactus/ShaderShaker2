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
    void AnnotationPrinter::Visit( const TranslationUnit & translation_unit )
    {
        m_Stream << "annotations:";
        VisitTable( *this, translation_unit.m_GlobalDeclarationTable, [&](VisitorBase<true> & visitor, const TranslationUnit::GlobalDeclarationType & table, const TranslationUnit::GlobalDeclarationType::const_iterator & current_iterator)
        {
            if ( current_iterator == table.begin() )
            {
                m_Stream << "{";
            }
            if ( table.end() - current_iterator == 1 )
            {
                m_Stream << "}";
            }
            if ( dynamic_cast<const VariableDeclaration* >( &**current_iterator ) && table.begin() != current_iterator ) {
                m_Stream << ",";
            }

            ( *current_iterator )->Visit( visitor );
        } );
        VisitTable( *this, translation_unit.m_TechniqueTable );
    }

}
