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
        m_Stream << type.m_Name;
    }

    void HLSLPrinter::Visit( UserDefinedType & type )
    {
        m_Stream << type.m_Name;
    }

    void HLSLPrinter::Visit( SamplerType & type )
    {
        m_Stream << type.m_Name;
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

        m_Stream << "( ";
        expression.m_Expression->Visit( *this );
        m_Stream << " )";
    }

    void HLSLPrinter::Visit( BinaryOperationExpression & expression )
    {
        m_Stream << "( ";
        expression.m_LeftExpression->Visit( *this );
        m_Stream << " ) ";

        switch( expression.m_Operation )
        {
            case BinaryOperationExpression::LogicalOr : m_Stream << "||"; break;
            case BinaryOperationExpression::LogicalAnd : m_Stream << "&&"; break;
            case BinaryOperationExpression::BitwiseOr : m_Stream << "|"; break;
            case BinaryOperationExpression::BitwiseXor : m_Stream << "^"; break;
            case BinaryOperationExpression::BitwiseAnd : m_Stream << "&"; break;
            case BinaryOperationExpression::Equality : m_Stream << "=="; break;
            case BinaryOperationExpression::Difference : m_Stream << "!="; break;
            case BinaryOperationExpression::LessThan : m_Stream << "<"; break;
            case BinaryOperationExpression::GreaterThan : m_Stream << ">"; break;
            case BinaryOperationExpression::LessThanOrEqual : m_Stream << "<="; break;
            case BinaryOperationExpression::GreaterThanOrEqual : m_Stream << ">="; break;
            case BinaryOperationExpression::BitwiseLeftShift : m_Stream << "<<"; break;
            case BinaryOperationExpression::BitwiseRightShift : m_Stream << ">>"; break;
            case BinaryOperationExpression::Addition : m_Stream << "+"; break;
            case BinaryOperationExpression::Subtraction : m_Stream << "-"; break;
            case BinaryOperationExpression::Multiplication : m_Stream << "*"; break;
            case BinaryOperationExpression::Division : m_Stream << "/"; break;
            case BinaryOperationExpression::Modulo : m_Stream << "%"; break;
            default : assert( !"Unsupported" );
        }

        m_Stream << " ( ";
        expression.m_RightExpression->Visit( *this );
        m_Stream << " )";
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

    void HLSLPrinter::Visit( ConstructorExpression & expression )
    {
        expression.m_Type->Visit( *this );
        m_Stream << "(";

        if( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }

        m_Stream << ")";
    }

    void HLSLPrinter::Visit( ConditionalExpression & expression )
    {
        //:TODO: generate parenthesis only when required ( using operator precedence )
        m_Stream << "( ";
        expression.m_Condition->Visit( *this );
        m_Stream << " ) ? ( ";
        expression.m_IfTrue->Visit( *this );
        m_Stream << " ) : ( ";
        expression.m_IfFalse->Visit( *this );
        m_Stream << " )";
    }

    void HLSLPrinter::Visit( LValueExpression & expression )
    {
        expression.m_VariableExpression->Visit( *this );

        if( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( PreModifyExpression & expression )
    {
        switch( expression.m_Operator )
        {
            case SelfModifyOperator_PlusPlus:
                m_Stream << "++";
                break;

            case SelfModifyOperator_MinusMinus:
                m_Stream << "--";
                break;

            default:
                assert( !"Unsupported enum" );
        }

        expression.m_Expression->Visit( *this );
    }

    void HLSLPrinter::Visit( PostModifyExpression & expression )
    {
        expression.m_Expression->Visit( *this );

        switch( expression.m_Operator )
        {
            case SelfModifyOperator_PlusPlus:
                m_Stream << "++";
                break;

            case SelfModifyOperator_MinusMinus:
                m_Stream << "--";
                break;

            default:
                assert( !"Unsupported enum" );
        }
    }

    void HLSLPrinter::Visit( CastExpression & expression )
    {
        m_Stream << "( ";
        expression.m_Type->Visit( *this );

        if( expression.m_ArraySize != -1 )
        {
            m_Stream << "[" << expression.m_ArraySize << "]";
        }

        m_Stream <<" )( ";

        expression.m_Expression->Visit( *this );

        m_Stream << " )";
    }

    void HLSLPrinter::Visit( ReturnStatement & statement )
    {
        m_Stream << "return";

        if( statement.m_Expression )
        {
            m_Stream << " ";
            statement.m_Expression->Visit( *this );
        }

        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( BreakStatement & statement )
    {
        m_Stream << "break;" << endl_ind;
    }

    void HLSLPrinter::Visit( ContinueStatement & statement )
    {
        m_Stream << "continue;" << endl_ind;
    }

    void HLSLPrinter::Visit( DiscardStatement & statement )
    {
        m_Stream << "discard;" << endl_ind;
    }

    void HLSLPrinter::Visit( EmptyStatement & statement )
    {
        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( ExpressionStatement & statement )
    {
        statement.m_Expression->Visit( *this );
        m_Stream << ";" << endl_ind;
    }

    void HLSLPrinter::Visit( IfStatement & statement )
    {
        m_Stream << "if( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " ) ";
        statement.m_ThenStatement->Visit( *this );

        if( statement.m_ElseStatement)
        {
            m_Stream << "else ";
            statement.m_ElseStatement->Visit( *this );
        }
    }

    void HLSLPrinter::Visit( WhileStatement & statement )
    {
        m_Stream << "while( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " ) ";
        statement.m_Statement->Visit( *this );
    }

    void HLSLPrinter::Visit( DoWhileStatement & statement )
    {
        m_Stream << "do ";
        statement.m_Statement->Visit( *this );
        m_Stream << "while( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " );\n";
    }

    void HLSLPrinter::Visit( BlockStatement & statement )
    {
        if( statement.m_StatementTable.empty() )
        {
            m_Stream << "{}" << endl_ind;
        }
        else
        {
            m_Stream << "{" << inc_ind << endl_ind;

            std::vector< std::shared_ptr<Statement> >::iterator
                it, end = statement.m_StatementTable.end();

            for ( it = statement.m_StatementTable.begin(); it != end; ++it )
            {
                (*it)->Visit( *this );
            }

            m_Stream << dec_ind << endl_ind << "}" << endl_ind;
        }
    }
}
