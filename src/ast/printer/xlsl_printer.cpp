#include "xlsl_printer.h"

#include <cassert>
#include <utils/indentation.h>
#include <ast/node.h>

namespace AST
{

    void XLSLPrinter::Visit( const Node & /*node*/ )
    {
        assert( !"Unsupported node type, implement in base class" );
    }

    void XLSLPrinter::Visit( const TranslationUnit & translation_unit )
    {
        AST::VisitTable( *this, translation_unit.m_GlobalDeclarationTable );
        AST::VisitTable( *this, translation_unit.m_TechniqueTable );
    }

    void XLSLPrinter::Visit( const VariableDeclaration & variable_declaration )
    {
        VisitTable( *this, variable_declaration.m_StorageClass, " ", false );

        if ( !variable_declaration.m_StorageClass.empty() )
            m_Stream << " ";

        VisitTable( *this, variable_declaration.m_TypeModifier, " ", false );

        if ( !variable_declaration.m_TypeModifier.empty() )
            m_Stream << " ";

        m_Stream << variable_declaration.m_Type->m_Name;

        m_Stream << inc_ind << endl_ind;
        VisitTable( *this, variable_declaration.m_BodyTable, ",", true );
        m_Stream << dec_ind;

        m_Stream << ";" << endl_ind;
    }

    void XLSLPrinter::Visit( const IntrinsicType & type )
    {
        m_Stream << type.m_Name;
    }

    void XLSLPrinter::Visit( const UserDefinedType & type )
    {
        m_Stream << type.m_Name;
    }

    void XLSLPrinter::Visit( const SamplerType & type )
    {
        m_Stream << type.m_Name;
    }

    void XLSLPrinter::Visit( const TypeModifier & modifier )
    {
        m_Stream << modifier.m_Value;
    }

    void XLSLPrinter::Visit( const StorageClass & storage_class )
    {
        m_Stream << storage_class.m_Value;
    }

    void XLSLPrinter::Visit( const ArgumentList & list )
    {
        VisitTable( *this, list.m_ArgumentTable, ", ", false );
    }

    void XLSLPrinter::Visit( const LiteralExpression & expression )
    {
        m_Stream << expression.m_Value;
    }

    void XLSLPrinter::Visit( const VariableExpression & expression )
    {
        m_Stream << expression.m_Name;

        if( expression.m_SubscriptExpression )
        {
            m_Stream << '[';
            expression.m_SubscriptExpression->Visit( *this );
            m_Stream << ']';
        }
    }

    void XLSLPrinter::Visit( const UnaryOperationExpression & expression )
    {
        m_Stream << expression.m_Operation << "( ";
        expression.m_Expression->Visit( *this );
        m_Stream << " )";
    }

    void XLSLPrinter::Visit( const BinaryOperationExpression & expression )
    {
        m_Stream << "( ";
        expression.m_LeftExpression->Visit( *this );
        m_Stream << " ) " << expression.m_Operation << " ( ";
        expression.m_RightExpression->Visit( *this );
        m_Stream << " )";
    }

    void XLSLPrinter::Visit( const CallExpression & expression )
    {
        m_Stream << expression.m_Name << "(";

        if( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }

        m_Stream << ")";
    }

    void XLSLPrinter::Visit( const ArgumentExpressionList & list )
    {
        VisitTable( *this, list.m_ExpressionList, ", ", false );
    }

    void XLSLPrinter::Visit( const Swizzle & swizzle )
    {
        m_Stream << "." << swizzle.m_Swizzle;
    }

    void XLSLPrinter::Visit( const PostfixSuffixCall & postfix_suffix )
    {
        m_Stream << ".";
        postfix_suffix.m_CallExpression->Visit( *this );

        if( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void XLSLPrinter::Visit( const PostfixSuffixVariable & postfix_suffix )
    {
        m_Stream << ".";
        postfix_suffix.m_VariableExpression->Visit( *this );

        if( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }
    }

    void XLSLPrinter::Visit( const ConstructorExpression & expression )
    {
        expression.m_Type->Visit( *this );
        m_Stream << "(";

        if( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }

        m_Stream << ")";
    }

    void XLSLPrinter::Visit( const ConditionalExpression & expression )
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

    void XLSLPrinter::Visit( const LValueExpression & expression )
    {
        expression.m_VariableExpression->Visit( *this );

        if( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void XLSLPrinter::Visit( const PreModifyExpression & expression )
    {
        m_Stream << expression.m_Operator;
        expression.m_Expression->Visit( *this );
    }

    void XLSLPrinter::Visit( const PostModifyExpression & expression )
    {
        expression.m_Expression->Visit( *this );
        m_Stream << expression.m_Operator;
    }

    void XLSLPrinter::Visit( const CastExpression & expression )
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

    void XLSLPrinter::Visit( const AssignmentExpression & expression )
    {
        expression.m_LValueExpression->Visit( *this );
        m_Stream << " " << expression.m_Operator << " ";
        expression.m_Expression->Visit( *this );
    }

    void XLSLPrinter::Visit( const PostfixExpression & expression )
    {
        expression.m_Expression->Visit( *this );

        if( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }
    }

    void XLSLPrinter::Visit( const ReturnStatement & statement )
    {
        m_Stream << "return";

        if( statement.m_Expression )
        {
            m_Stream << " ";
            statement.m_Expression->Visit( *this );
        }

        m_Stream << ";" << endl_ind;
    }

    void XLSLPrinter::Visit( const BreakStatement & /*statement*/ )
    {
        m_Stream << "break;" << endl_ind;
    }

    void XLSLPrinter::Visit( const ContinueStatement & /*statement*/ )
    {
        m_Stream << "continue;" << endl_ind;
    }

    void XLSLPrinter::Visit( const EmptyStatement & /*statement*/ )
    {
        m_Stream << ";" << endl_ind;
    }

    void XLSLPrinter::Visit( const ExpressionStatement & statement )
    {
        statement.m_Expression->Visit( *this );
        m_Stream << ";" << endl_ind;
    }

    void XLSLPrinter::Visit( const IfStatement & statement )
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

    void XLSLPrinter::Visit( const WhileStatement & statement )
    {
        m_Stream << "while( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " ) ";
        statement.m_Statement->Visit( *this );
    }

    void XLSLPrinter::Visit( const DoWhileStatement & statement )
    {
        m_Stream << "do ";
        statement.m_Statement->Visit( *this );
        m_Stream << "while( ";
        statement.m_Condition->Visit( *this );
        m_Stream << " );\n";
    }

    void XLSLPrinter::Visit( const BlockStatement & statement )
    {
        if( statement.m_StatementTable.empty() )
        {
            m_Stream << "{}" << endl_ind;
        }
        else
        {
            m_Stream << "{" << inc_ind << endl_ind;

            AST::VisitTable( *this, statement.m_StatementTable );

            m_Stream << dec_ind << endl_ind << "}" << endl_ind;
        }
    }

    void XLSLPrinter::Visit( const AssignmentStatement & statement )
    {
        statement.m_Expression->Visit( *this );
        m_Stream << ";" << endl_ind;
    }

    void XLSLPrinter::Visit( const VariableDeclarationStatement & statement )
    {
        VisitTable( *this, statement.m_StorageClass, " ", false );

        if ( !statement.m_StorageClass.empty() )
            m_Stream << " ";

        VisitTable( *this, statement.m_TypeModifier, " ", false );

        if ( !statement.m_TypeModifier.empty() )
            m_Stream << " ";

        m_Stream << statement.m_Type->m_Name;

        m_Stream << inc_ind << endl_ind;
        VisitTable( *this, statement.m_BodyTable, ",", true );
        m_Stream << dec_ind;
        m_Stream << ";" << endl_ind;
    }
}
