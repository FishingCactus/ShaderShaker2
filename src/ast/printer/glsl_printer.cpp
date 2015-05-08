#include "glsl_printer.h"

#include <cassert>
#include <utils/indentation.h>
#include <ast/node.h>

namespace AST
{
    GLSLPrinter::GLSLPrinter( std::ostream & stream )
        : XLSLPrinter( stream )
    {
        InitializeTypeLookupTable();

    }

    void GLSLPrinter::Visit( const VariableDeclarationBody & body )
    {
        m_Stream << body.m_Name;

        if ( body.m_ArraySize != -1 )
        {
            m_Stream << "[" << body.m_ArraySize << "]";
        }

        if ( body.m_InitialValue )
        {
            m_Stream << " = ";

            body.m_InitialValue->Visit( *this );
        }
    }

    void GLSLPrinter::Visit( const IntrinsicType & type )
    {
        TypeLookupMap::const_iterator it;

        it = m_TypeLookupMap.find( type.m_Name );

        if ( it == m_TypeLookupMap.end() )
        {
            m_Stream << type.m_Name;
        }
        else
        {
            m_Stream << it->second;
        }
    }

    void GLSLPrinter::Visit( const InitialValue & initial_value )
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

    void GLSLPrinter::Visit( const Annotations & /*annotations*/ )
    {
    }

    void GLSLPrinter::Visit( const AnnotationEntry & /*annotation_entry*/ )
    {
    }

    void GLSLPrinter::Visit( const TextureDeclaration & declaration )
    {
        m_Stream << declaration.m_Type << " " << declaration.m_Name;

        m_Stream << ";" << endl_ind;
    }

    void GLSLPrinter::Visit( const SamplerDeclaration & declaration )
    {
        m_Stream << declaration.m_Type << " "
            << declaration.m_Name << endl_ind
            << "{" << inc_ind << endl_ind;

        AST::VisitTable( *this, declaration.m_BodyTable );

        m_Stream << dec_ind << endl_ind << "}" << endl_ind;
    }

    void GLSLPrinter::Visit( const SamplerBody & body )
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

    void GLSLPrinter::Visit( const StructDefinition & definition )
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
            }
        }

    }

    void GLSLPrinter::Visit( const FunctionDeclaration & declaration )
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

    void GLSLPrinter::Visit( const Argument & argument )
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

    void GLSLPrinter::Visit( const DiscardStatement & /*statement*/ )
    {
        m_Stream << "discard;" << endl_ind;
    }

    void GLSLPrinter::InitializeTypeLookupTable()
    {
        // :TODO: support multiple GLSL versions

        m_TypeLookupMap[ "int4" ] = "vec4";
        m_TypeLookupMap[ "float2" ] = "vec2";
        m_TypeLookupMap[ "float3" ] = "vec3";
        m_TypeLookupMap[ "float4" ] = "vec4";
        m_TypeLookupMap[ "float4x4" ] = "mat4";
        m_TypeLookupMap[ "float4x3" ] = "mat4";
    }
}
