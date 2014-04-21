#include "hlsl_printer.h"

#include <cassert>
#include "utils/indentation.h"
#include "ast/node.h"

namespace AST
{

    void HLSLPrinter::Visit( Node & node )
    {
        assert( !"Unsupported node type, implement in base class" );
    }

    void HLSLPrinter::Visit( TranslationUnit & translation_unit )
    {


    }

    void HLSLPrinter::Visit( VariableDeclaration & variable_declaration )
    {


    }

    void HLSLPrinter::Visit( IntrinsicType & type )
    {


    }

    void HLSLPrinter::Visit( UserDefinedType & type )
    {


    }

    void HLSLPrinter::Visit( SamplerType & type )
    {


    }

    void HLSLPrinter::Visit( TypeModifier & modifier )
    {


    }

    void HLSLPrinter::Visit( StorageClass & storage_class )
    {


    }

    void HLSLPrinter::Visit( VariableDeclarationBody & body )
    {


    }

    void HLSLPrinter::Visit( TextureDeclaration & declaration )
    {


    }

    void HLSLPrinter::Visit( SamplerDeclaration & declaration )
    {


    }

    void HLSLPrinter::Visit( SamplerBody & body )
    {


    }

    void HLSLPrinter::Visit( StructDefinition & definition )
    {


    }

    void HLSLPrinter::Visit( LiteralExpression & expression )
    {

        m_Stream << expression.m_Value;
    }

    void HLSLPrinter::Visit( VariableExpression & expression )
    {
        m_Stream << expression.m_Name;

        if( expression.m_SubscriptExpression )
        {
            m_Stream << '[';
            expression.m_SubscriptExpression->Visit( *this );
            m_Stream << ']';
        }
    }

}
