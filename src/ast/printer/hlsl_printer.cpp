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

    void HLSLPrinter::Visit( UnaryOperationExpression & expression )
    {
        switch( expression.m_Operation )
        {
            case UnaryOperationExpression::Plus:
                m_Stream << "+";
                break;

            case UnaryOperationExpression::Minus:
                m_Stream << "-";
                break;

            case UnaryOperationExpression::Not:
                m_Stream << "!";
                break;

            case UnaryOperationExpression::BitwiseNot:
                m_Stream << "~";
                break;

            default:
                assert( !"Unsupported operator" );
        }

        expression.m_Expression->Visit( *this );
    }

    void HLSLPrinter::Visit( CallExpression & expression )
    {
        m_Stream << expression.m_Name << "(";

        if( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }

        m_Stream << ")";
    }

    void HLSLPrinter::Visit( ArgumentExpressionList & list )
    {
        size_t
            index,
            count;

        count = list.m_ExpressionList.size();

        for( index = 0; index < count; ++index )
        {
            if( index != 0 ) m_Stream << ", ";
            list.m_ExpressionList[ index ]->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( Swizzle & swizzle )
    {
        m_Stream << "." << swizzle.m_Swizzle;
    }

    void HLSLPrinter::Visit( PostfixSuffixCall & postfix_suffix )
    {
        m_Stream << ".";
        postfix_suffix.m_CallExpression->Visit( *this );

        if( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( PostfixSuffixVariable & postfix_suffix )
    {
        m_Stream << ".";
        postfix_suffix.m_VariableExpression->Visit( *this );

        if( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

}
