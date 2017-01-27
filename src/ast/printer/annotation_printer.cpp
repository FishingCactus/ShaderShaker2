#include "annotation_printer.h"

#include <cassert>
#include <utils/indentation.h>
#include <utils/string_helper.h>
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

        VisitTable( *this, annotations.m_AnnotationTable, separator );

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

        std::string
            annotation_value;

        if ( ( annotation_entry.m_Value[0] == '"' || annotation_entry.m_Value[0] == '\'' )
        && ( annotation_entry.m_Value[annotation_entry.m_Value.length() - 1] == '"' || annotation_entry.m_Value[annotation_entry.m_Value.length() - 1] == '\'' ) )
        {
            annotation_value = annotation_entry.m_Value.substr(1);
            annotation_value = annotation_value.substr(0, annotation_value.length() - 1);
        }
        else
        {
            annotation_value = annotation_entry.m_Value;
        }

        tolower_in_place( annotation_value );

        m_Stream
            << "\"" << annotation_entry.m_Name << "\":"
            << "{"
                << "\"type\":\"" << annotation_entry.m_Type << "\","
                << "\"value\":\"" << annotation_value << "\""
            << "}";
    }

    void AnnotationPrinter::Visit( const VariableDeclarationBody & body )
    {
        if ( body.m_Annotations && body.m_Name != "" )
        {
            m_Stream << "\"" << body.m_Name << "\":";
            m_Stream << "{";
            TreeTraverser::Visit(body);
            m_Stream << "}";
        }
        // :NOTE: No need to traverse the tree further, there are no annotations for this entry.
    }

    void AnnotationPrinter::Visit( const InitialValue & initial_value )
    {
        m_Stream << ",\"InitialValue\":";
        m_InsideInitialValue = true;
        // :TODO: Support the m_Vector property for initializers like = { 1.0f, 1.0f, 1.0f }
        TreeTraverser::Visit(initial_value);
        m_InsideInitialValue = false;
    }

    void AnnotationPrinter::Visit( const ConstructorExpression & expression )
    {
        if ( m_InsideInitialValue )
        {
            m_Stream << "{";
            m_Stream << "\"Constructor\":\"" << expression.m_Type->m_Name << "\"";
            TreeTraverser::Visit(expression);
            m_Stream << "}";
        }

    }

    void AnnotationPrinter::Visit( const ArgumentExpressionList & list )
    {
        if ( m_InsideInitialValue )
        {
            m_Stream << ", \"Arguments\":";
            VisitTable( *this, list.m_ExpressionList, [&](VisitorBase<true> & visitor, const ArgumentExpressionList::VariableExpressionListType & table, const ArgumentExpressionList::VariableExpressionListType::const_iterator & current_iterator)
            {
                if ( current_iterator == table.begin() )
                {
                    m_Stream << "[";
                }
                else
                {
                    m_Stream << ",";
                }

                ( * current_iterator )->Visit( visitor );

                if ( table.end() - current_iterator == 1 )
                {
                    m_Stream << "]";
                }
            } );
        }
    }

    void AnnotationPrinter::Visit( const LiteralExpression & expression )
    {
        if ( m_InsideInitialValue )
        {
            m_Stream << "\"" << expression.m_Value << "\"";
        }
    }

    void AnnotationPrinter::Visit( const TextureDeclaration & body )
    {
        if ( body.m_Annotations && body.m_Name != "" )
        {
            m_Stream << "\"" << body.m_Name << "\":";
        }

        TreeTraverser::Visit(body);
    }

    void AnnotationPrinter::Visit( const TranslationUnit & translation_unit )
    {
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
            if ( ( dynamic_cast<const VariableDeclaration* >( &**current_iterator )
                || dynamic_cast<const TextureDeclaration* >( &**current_iterator ) )
                && table.begin() != current_iterator )
            {
                m_Stream << ",";
            }

            ( *current_iterator )->Visit( visitor );
        } );
        VisitTable( *this, translation_unit.m_TechniqueTable );
    }

}
