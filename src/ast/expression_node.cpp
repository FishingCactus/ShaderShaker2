#include "node.h"
#include <cassert>

namespace AST
{

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
}