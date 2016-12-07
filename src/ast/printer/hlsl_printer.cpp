#include "hlsl_printer.h"

#include <cassert>
#include <utils/indentation.h>
#include <ast/node.h>

namespace AST
{
    void HLSLPrinter::Visit( const VariableDeclarationBody & body )
    {
        m_Stream << body.m_Name;

        if( body.m_ArraySize )
        {
            m_Stream << "[" << body.m_ArraySize << "]";
        }

        if( !body.m_Semantic.empty() )
        {
            m_Stream << " : " << body.m_Semantic;
        }

        if( body.m_Annotations )
        {
            body.m_Annotations->Visit( *this );
        }

        if( body.m_InitialValue )
        {
            m_Stream << " = ";

            body.m_InitialValue->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const InitialValue & initial_value )
    {
        if ( initial_value.m_Vector )
        {
            m_Stream << "{ ";
            VisitTable( *this, initial_value.m_ExpressionTable, ", ", false );
            m_Stream << " }";
        }
        else
        {
            assert( initial_value.m_ExpressionTable.size() == 1 );
            initial_value.m_ExpressionTable[ 0 ]->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const Annotations & annotations )
    {
        m_Stream << " < ";
        AST::VisitTable( *this, annotations.m_AnnotationTable );
        m_Stream << ">";
    }

    void HLSLPrinter::Visit( const AnnotationEntry & annotation_entry )
    {
        m_Stream << annotation_entry.m_Type << " " << annotation_entry.m_Name
            << " = " << annotation_entry.m_Value << "; ";
    }

    void HLSLPrinter::Visit( const TextureDeclaration & declaration )
    {
        m_Stream << declaration.m_Type << " " << declaration.m_Name;

        if( !declaration.m_Semantic.empty() )
        {
            m_Stream << " : " << declaration.m_Semantic;
        }

        if( declaration.m_Annotations )
        {
            declaration.m_Annotations->Visit( *this );
        }

        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( const SamplerDeclaration & declaration )
    {
        m_Stream << declaration.m_Type << " "
            << declaration.m_Name << endl_ind
            << "{" << inc_ind << endl_ind;

        AST::VisitTable( *this, declaration.m_BodyTable );

        m_Stream << dec_ind << endl_ind << "}" << endl_ind;
    }

    void HLSLPrinter::Visit( const SamplerBody & body )
    {
        if( body.m_Name == "texture" )
        {
            m_Stream << "texture=<" << body.m_Value << ">";
        }
        else
        {
            m_Stream << body.m_Name << " = " << body.m_Value;
        }

        m_Stream << ";" << endl_ind;

    }

    void HLSLPrinter::Visit( const StructDefinition & definition )
    {
        m_Stream << " struct " << definition.m_Name << endl_ind;
        m_Stream << "{" << inc_ind << endl_ind;

        {
            std::vector< StructDefinition::Member >::const_iterator it, end;
            it = definition.m_MemberTable.cbegin();
            end = definition.m_MemberTable.cend();

            for(; it!=end; ++it )
            {
                const StructDefinition::Member
                    & member = (*it);

                if( !member.m_InterpolationModifier.empty() )
                {
                    m_Stream << member.m_InterpolationModifier << " ";
                }

                member.m_Type->Visit( *this );

                m_Stream << " " << member.m_Name;

                if( !member.m_Semantic.empty() )
                {
                    m_Stream << " : " <<member.m_Semantic;
                }

                if( end - it == 1 )
                {
                    m_Stream << dec_ind;
                }

                m_Stream << ";" << endl_ind;
            }
        }
        m_Stream << "}" << endl_ind;

    }

    void HLSLPrinter::Visit( const FunctionDeclaration & declaration )
    {
        AST::VisitTable( *this, declaration.m_StorageClassTable );

        if( declaration.m_Type )
        {
            declaration.m_Type->Visit( *this );
        }
        else
        {
            m_Stream << "void";
        }

        m_Stream << " " << declaration.m_Name << "(";

        if( declaration.m_ArgumentList )
        {
            declaration.m_ArgumentList->Visit( * this );
        }

        m_Stream << ")";

        if( !declaration.m_Semantic.empty() )
        {
            m_Stream << " : " << declaration.m_Semantic;
        }

        m_Stream << endl_ind << "{" << inc_ind << endl_ind;

        AST::VisitTable( *this, declaration.m_StatementTable );

        m_Stream << dec_ind << endl_ind << "}" << endl_ind;
    }

    void HLSLPrinter::Visit( const Argument & argument )
    {
        if( !argument.m_InputModifier.empty() )
        {
            m_Stream << argument.m_InputModifier << " ";
        }

        if( argument.m_TypeModifier )
        {
            argument.m_TypeModifier->Visit( *this );
        }

        m_Stream << argument.m_Type->m_Name << " " << argument.m_Name;

        if( !argument.m_Semantic.empty() )
        {
            m_Stream << " : " << argument.m_Semantic;
        }

        if( !argument.m_InterpolationModifier.empty() )
        {
            m_Stream << " " << argument.m_InterpolationModifier;
        }

        if( argument.m_InitialValue )
        {
            m_Stream << " = ";

            argument.m_InitialValue->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( const DiscardStatement & /*statement*/ )
    {
        m_Stream << "discard;" << endl_ind;
    }
}
