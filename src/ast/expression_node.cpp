#include "node.h"
#include <cassert>

namespace AST
{
    #define CloneTable( _Type_, _member_ ) \
    { \
        std::vector< Base::ObjectRef<_Type_> >::const_iterator it, end; \
        \
        it = _member_.begin(); \
        end = _member_.end(); \
        for( ; it!=end; ++it ) \
        { \
            clone->_member_.push_back( (*it)->Clone() ); \
        } \
    }

    std::ostream& operator<<(
        std::ostream &out,
        SelfModifyOperator self_modify_operator
        )
    {
        switch( self_modify_operator )
        {
            case SelfModifyOperator_PlusPlus: out << "++"; break;
            case SelfModifyOperator_MinusMinus: out << "--"; break;
            default: assert( !"Unsupported enum" );
        }

        return out;
    }

    std::ostream& operator<<(
        std::ostream &out,
        BinaryOperationExpression::Operation operation
        )
    {
        switch( operation )
        {
            case BinaryOperationExpression::LogicalOr : out << "||"; break;
            case BinaryOperationExpression::LogicalAnd : out << "&&"; break;
            case BinaryOperationExpression::BitwiseOr : out << "|"; break;
            case BinaryOperationExpression::BitwiseXor : out << "^"; break;
            case BinaryOperationExpression::BitwiseAnd : out << "&"; break;
            case BinaryOperationExpression::Equality : out << "=="; break;
            case BinaryOperationExpression::Difference : out << "!="; break;
            case BinaryOperationExpression::LessThan : out << "<"; break;
            case BinaryOperationExpression::GreaterThan : out << ">"; break;
            case BinaryOperationExpression::LessThanOrEqual : out << "<="; break;
            case BinaryOperationExpression::GreaterThanOrEqual : out << ">="; break;
            case BinaryOperationExpression::BitwiseLeftShift : out << "<<"; break;
            case BinaryOperationExpression::BitwiseRightShift : out << ">>"; break;
            case BinaryOperationExpression::Addition : out << "+"; break;
            case BinaryOperationExpression::Subtraction : out << "-"; break;
            case BinaryOperationExpression::Multiplication : out << "*"; break;
            case BinaryOperationExpression::Division : out << "/"; break;
            case BinaryOperationExpression::Modulo : out << "%"; break;
            default : assert( !"Unsupported" );
        }

        return out;
    }

    std::ostream& operator<<(
        std::ostream &out,
        UnaryOperationExpression::Operation operation
        )
    {
        switch( operation )
        {
            case UnaryOperationExpression::Plus: out << "+"; break;
            case UnaryOperationExpression::Minus: out << "-"; break;
            case UnaryOperationExpression::Not: out << "!"; break;
            case UnaryOperationExpression::BitwiseNot: out << "~"; break;
            default : assert( !"Unsupported operator" );
        }

        return out;
    }

    std::ostream& operator<<(
        std::ostream &out,
        AssignmentOperator assignment_operator
        )
    {
        switch( assignment_operator )
        {
            case AssignmentOperator_Assign : out << "="; break;
            case AssignmentOperator_Multiply : out << "*="; break;
            case AssignmentOperator_Divide : out << "/="; break;
            case AssignmentOperator_Add : out << "+="; break;
            case AssignmentOperator_Subtract : out << "-="; break;
            case AssignmentOperator_BitwiseAnd : out << "&="; break;
            case AssignmentOperator_BitwiseOr : out << "|="; break;
            case AssignmentOperator_BitwiseXor : out << "^="; break;
            case AssignmentOperator_LeftShift : out << "<<="; break;
            case AssignmentOperator_RightShift : out << ">>="; break;
            default : assert( !"Unsupported" );
        }

        return out;
    }

    ConditionalExpression * ConditionalExpression::Clone() const
    {
        ConditionalExpression * clone = new ConditionalExpression;

        clone->m_Condition = m_Condition->Clone();
        clone->m_IfTrue = m_IfTrue->Clone();
        clone->m_IfFalse = m_IfFalse->Clone();

        return clone;
    }

    BinaryOperationExpression * BinaryOperationExpression::Clone() const
    {
        BinaryOperationExpression * clone = new BinaryOperationExpression;


        clone->m_Operation = m_Operation;
        clone->m_LeftExpression = m_LeftExpression->Clone();
        clone->m_RightExpression = m_RightExpression->Clone();

        return clone;
    }

    UnaryOperationExpression * UnaryOperationExpression::Clone() const
    {
        UnaryOperationExpression * clone = new UnaryOperationExpression;

        clone->m_Operation = m_Operation;
        clone->m_Expression = m_Expression->Clone();

        return clone;
    }

    CastExpression * CastExpression::Clone() const
    {
        CastExpression * clone = new CastExpression;

        clone->m_Type = m_Type->Clone();
        clone->m_ArraySize = m_ArraySize;
        clone->m_Expression = m_Expression->Clone();

        return clone;
    }

    LiteralExpression * LiteralExpression::Clone() const
    {
        LiteralExpression * clone = new LiteralExpression;

        clone->m_Type = m_Type;
        clone->m_Value = m_Value;

        return clone;
    }

    VariableExpression * VariableExpression::Clone() const
    {
        VariableExpression * clone = new VariableExpression;

        clone->m_Name = m_Name;

        if( m_SubscriptExpression )
        {
            clone->m_SubscriptExpression = m_SubscriptExpression->Clone();
        }

        return clone;
    }

    PostfixExpression * PostfixExpression::Clone() const
    {
        PostfixExpression * clone = new PostfixExpression;

        clone->m_Expression = m_Expression->Clone();

        if( m_Suffix )
        {
            clone->m_Suffix = m_Suffix->Clone();
        }

        return clone;
    }

    ArgumentExpressionList * ArgumentExpressionList::Clone() const
    {
        ArgumentExpressionList * clone = new ArgumentExpressionList;

        CloneTable( Expression, m_ExpressionList )

        return clone;
    }

    CallExpression * CallExpression::Clone() const
    {
        CallExpression * clone = new CallExpression;

        clone->m_Name = m_Name;

        if( m_ArgumentExpressionList )
        {
            clone->m_ArgumentExpressionList = m_ArgumentExpressionList->Clone();
        }

        return clone;
    }

    ConstructorExpression * ConstructorExpression::Clone() const
    {
        ConstructorExpression * clone = new ConstructorExpression;

        clone->m_Type = m_Type->Clone();

        if( m_ArgumentExpressionList )
        {
            clone->m_ArgumentExpressionList = m_ArgumentExpressionList->Clone();
        }

        return clone;
    }

    Swizzle * Swizzle::Clone() const
    {
        Swizzle * clone = new Swizzle;

        clone->m_Swizzle = m_Swizzle;

        return clone;
    }

    PostfixSuffixCall * PostfixSuffixCall::Clone() const
    {
        PostfixSuffixCall * clone = new PostfixSuffixCall;

        clone->m_CallExpression = m_CallExpression->Clone();

        if( m_Suffix )
        {
            clone->m_Suffix = m_Suffix->Clone();
        }

        return clone;
    }

    PostfixSuffixVariable * PostfixSuffixVariable::Clone() const
    {
        PostfixSuffixVariable * clone = new PostfixSuffixVariable;

        clone->m_VariableExpression = m_VariableExpression->Clone();

        if ( m_Suffix )
        {
            clone->m_Suffix = m_Suffix->Clone();
        }

        return clone;
    }


    LValueExpression * LValueExpression::Clone() const
    {
        LValueExpression * clone = new LValueExpression;

        clone->m_VariableExpression = m_VariableExpression->Clone();

        if( m_Suffix )
        {
            clone->m_Suffix = m_Suffix->Clone();
        }

        return clone;
    }

    PreModifyExpression * PreModifyExpression::Clone() const
    {
        PreModifyExpression * clone = new PreModifyExpression;

        clone->m_Operator = m_Operator;
        clone->m_Expression = m_Expression->Clone();

        return clone;
    }

    PostModifyExpression * PostModifyExpression::Clone() const
    {
        PostModifyExpression * clone = new PostModifyExpression;

        clone->m_Operator = m_Operator;
        clone->m_Expression = m_Expression->Clone();

        return clone;
    }

    AssignmentExpression * AssignmentExpression::Clone() const
    {
        AssignmentExpression * clone = new AssignmentExpression;

        clone->m_LValueExpression = m_LValueExpression->Clone();
        clone->m_Operator = m_Operator;
        clone->m_Expression = m_Expression->Clone();

        return clone;
    }
}