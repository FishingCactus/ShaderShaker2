#include "catch.hpp"
#include "ast/node.h"
#include "ast/printer/hlsl_printer.h"
#include <sstream>

TEST_CASE( "Literals are printed", "[ast][hlsl][printer]" )
{

    SECTION( "Integers are printed" )
    {
        AST::LiteralExpression
            node( AST::LiteralExpression::Int, "123" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "123" );
    }

    SECTION( "Floats are printed" )
    {
        AST::LiteralExpression
            node( AST::LiteralExpression::Float, "123.4f" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "123.4f" );
    }

    SECTION( "Booleans are printed" )
    {
        AST::LiteralExpression
            node( AST::LiteralExpression::Int, "true" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "true" );
    }
}

TEST_CASE( "Variables are printed", "[ast][hlsl][printer]" )
{

    SECTION( "Simple variables are printed" )
    {
        AST::VariableExpression
            node( "test" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "test" );
    }

    SECTION( "Float are printed" )
    {
        AST::VariableExpression
            node( "test" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.m_SubscriptExpression = new AST::LiteralExpression( AST::LiteralExpression::Int, "1" );

        node.Visit( printer );

        CHECK( output.str() == "test[1]" );
    }
}

TEST_CASE( "Unary Operators are printed", "[ast][hlsl][printer]" )
{

    SECTION( "+ operator is printed" )
    {
        AST::UnaryOperationExpression
            node( AST::UnaryOperationExpression::Plus, new AST::LiteralExpression( AST::LiteralExpression::Int, "1" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "+( 1 )" );
    }

    SECTION( "- operator is printed" )
    {
        AST::UnaryOperationExpression
            node( AST::UnaryOperationExpression::Minus, new AST::LiteralExpression( AST::LiteralExpression::Int, "1" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "-( 1 )" );
    }

    SECTION( "! operator is printed" )
    {
        AST::UnaryOperationExpression
            node( AST::UnaryOperationExpression::Not, new AST::LiteralExpression( AST::LiteralExpression::Int, "1" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "!( 1 )" );
    }

    SECTION( "~ operator is printed" )
    {
        AST::UnaryOperationExpression
            node( AST::UnaryOperationExpression::BitwiseNot, new AST::LiteralExpression( AST::LiteralExpression::Int, "1" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "~( 1 )" );
    }

}

TEST_CASE( "Call expressions are printed", "[ast][hlsl][printer]" )
{

    SECTION( "Simple call is printed" )
    {
        AST::CallExpression
            node( "test", 0 );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "test()" );
    }

    SECTION( "Call with arguments is printed" )
    {
        AST::ArgumentExpressionList
            * argument_list;

        argument_list = new AST::ArgumentExpressionList();
        argument_list->AddExpression( new AST::VariableExpression( "a" ) );
        argument_list->AddExpression( new AST::VariableExpression( "b" ) );
        argument_list->AddExpression( new AST::VariableExpression( "c" ) );

        AST::CallExpression
            node( "test", argument_list );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "test(a, b, c)" );
    }
}

TEST_CASE( "Postfix Suffixes are printed", "[ast][hlsl][printer]" )
{

    SECTION( "Swizzle is printed" )
    {
        AST::Swizzle
            node( "xyzw" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == ".xyzw" );
    }

    SECTION( "Postfix Suffix Call is printed" )
    {
        AST::PostfixSuffixCall
            node( new AST::CallExpression( "test", 0 ), 0 );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == ".test()" );
    }

    SECTION( "Postfix Suffix Call is printed" )
    {
        AST::PostfixSuffixCall
            node( new AST::CallExpression( "test", 0 ), new AST::Swizzle( "xxxx" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == ".test().xxxx" );
    }

    SECTION( "Postfix Suffix Variable is printed" )
    {
        AST::PostfixSuffixVariable
            node( new AST::VariableExpression( "test" ), 0 );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == ".test" );
    }

    SECTION( "Postfix Suffix Variable is printed" )
    {
        AST::PostfixSuffixVariable
            node( new AST::VariableExpression( "test" ), new AST::Swizzle( "xxxx" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == ".test.xxxx" );
    }
}

TEST_CASE( "Constructors are printed", "[ast][hlsl][printer]" )
{

    SECTION( "Simple constructor is printed" )
    {
        AST::ConstructorExpression
            node( new AST::IntrinsicType( "float4" ), 0 );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "float4()" );
    }

    SECTION( "Constructor with arguments is printed" )
    {
        AST::ArgumentExpressionList
            * argument_list;

        argument_list = new AST::ArgumentExpressionList();
        argument_list->AddExpression( new AST::VariableExpression( "a" ) );
        argument_list->AddExpression( new AST::VariableExpression( "b" ) );
        argument_list->AddExpression( new AST::VariableExpression( "c" ) );
        argument_list->AddExpression( new AST::VariableExpression( "d" ) );

        AST::ConstructorExpression
            node( new AST::IntrinsicType( "float4" ), argument_list );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "float4(a, b, c, d)" );
    }
}

TEST_CASE( "Conditionnals expression are printed", "[ast][hlsl][printer]" )
{

    AST::ConditionalExpression
        node;
    std::ostringstream
        output;
    AST::HLSLPrinter
        printer( output );

    node.m_Condition = new AST::VariableExpression( "a" );
    node.m_IfTrue = new AST::VariableExpression( "b" );
    node.m_IfFalse = new AST::VariableExpression( "c" );

    node.Visit( printer );

    CHECK( output.str() == "( a ) ? ( b ) : ( c )" );
}

TEST_CASE( "Types are printed", "[ast][hlsl][printer]" )
{
    SECTION( "Intrinsic type is printed" )
    {
        AST::IntrinsicType
            node( "float4" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "float4" );
    }

    SECTION( "User defined type is printed" )
    {
        AST::UserDefinedType
            node( "VS_INPUT" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "VS_INPUT" );
    }

    SECTION( "Sampler type is printed" )
    {
        AST::IntrinsicType
            node( "texture2D" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "texture2D" );
    }
}

TEST_CASE( "LValue expression are printed", "[ast][hlsl][printer]" )
{
    SECTION( "LValue without postfix is printed" )
    {
        AST::LValueExpression
            node( new AST::VariableExpression( "X" ), 0 );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "X" );
    }

    SECTION( "LValue with postfix is printed" )
    {
        AST::LValueExpression
            node( new AST::VariableExpression( "X" ), new AST::Swizzle( "xyzw" ) );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "X.xyzw" );
    }
}

TEST_CASE( "Premodify expression are printed", "[ast][hlsl][printer]" )
{
    SECTION( "++X is printed" )
    {
        AST::PreModifyExpression
            node(
                AST::SelfModifyOperator_PlusPlus,
                new AST::LValueExpression( new AST::VariableExpression( "X" ), 0 )
                );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "++X" );
    }

    SECTION( "--X is printed" )
    {
        AST::PreModifyExpression
            node(
                AST::SelfModifyOperator_MinusMinus,
                new AST::LValueExpression( new AST::VariableExpression( "X" ), 0 )
                );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "--X" );
    }
}

TEST_CASE( "Postmodify expression are printed", "[ast][hlsl][printer]" )
{
    SECTION( "X++ is printed" )
    {
        AST::PostModifyExpression
            node(
                AST::SelfModifyOperator_PlusPlus,
                new AST::LValueExpression( new AST::VariableExpression( "X" ), 0 )
                );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "X++" );
    }

    SECTION( "--X is printed" )
    {
        AST::PostModifyExpression
            node(
                AST::SelfModifyOperator_MinusMinus,
                new AST::LValueExpression( new AST::VariableExpression( "X" ), 0 )
                );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "X--" );
    }
}

std::string PrintBinaryOperator( AST::BinaryOperationExpression::Operation operation )
{
    AST::BinaryOperationExpression
        node(
            operation,
            new AST::VariableExpression( "X" ),
            new AST::VariableExpression( "Y" )
            );
    std::ostringstream
        output;
    AST::HLSLPrinter
        printer( output );

    node.Visit( printer );

    return output.str();
}

TEST_CASE( "Binary operations are printed", "[ast][hlsl][printer]" )
{
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::LogicalOr ) == "( X ) || ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::LogicalAnd ) == "( X ) && ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::BitwiseOr ) == "( X ) | ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::BitwiseXor ) == "( X ) ^ ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::BitwiseAnd ) == "( X ) & ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::Equality ) == "( X ) == ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::Difference ) == "( X ) != ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::LessThan ) == "( X ) < ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::GreaterThan ) == "( X ) > ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::LessThanOrEqual ) == "( X ) <= ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::GreaterThanOrEqual ) == "( X ) >= ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::BitwiseLeftShift ) == "( X ) << ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::BitwiseRightShift ) == "( X ) >> ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::Addition ) == "( X ) + ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::Subtraction ) == "( X ) - ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::Multiplication ) == "( X ) * ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::Division ) == "( X ) / ( Y )" );
    CHECK( PrintBinaryOperator( AST::BinaryOperationExpression::Modulo ) == "( X ) % ( Y )" );
}

TEST_CASE( "Cast expression is printed", "[ast][hlsl][printer]" )
{
    SECTION( "Simple type cast is printed" )
    {
        AST::CastExpression
            node(
                new AST::UserDefinedType( "SomeType" ),
                -1,
                new AST::VariableExpression( "X" )
                );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "( SomeType )( X )" );
    }

    SECTION( "Array like cast is printed" )
    {
        AST::CastExpression
            node(
                new AST::UserDefinedType( "SomeType" ),
                123,
                new AST::VariableExpression( "X" )
                );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "( SomeType[123] )( X )" );
    }
}

std::string PrintAssignmentExpression( AST::AssignmentOperator operation )
{
    AST::AssignmentExpression
        node(
            new AST::LValueExpression( new AST::VariableExpression( "X" ), 0 ),
            operation,
            new AST::VariableExpression( "Y" )
            );
    std::ostringstream
        output;
    AST::HLSLPrinter
        printer( output );

    node.Visit( printer );

    return output.str();
}

TEST_CASE( "Assignment expression are printed", "[ast][hlsl][printer]" )
{
    CHECK( PrintAssignmentExpression( AST::AssignmentOperator_Assign ) == "X = Y" );
    CHECK( PrintAssignmentExpression( AST::AssignmentOperator_Multiply ) == "X *= Y" );
    CHECK( PrintAssignmentExpression( AST::AssignmentOperator_Divide ) == "X /= Y" );
    CHECK( PrintAssignmentExpression( AST::AssignmentOperator_Add ) == "X += Y" );
    CHECK( PrintAssignmentExpression( AST::AssignmentOperator_Subtract ) == "X -= Y" );
    CHECK( PrintAssignmentExpression( AST::AssignmentOperator_BitwiseAnd ) == "X &= Y" );
    CHECK( PrintAssignmentExpression( AST::AssignmentOperator_BitwiseOr ) == "X |= Y" );
    CHECK( PrintAssignmentExpression( AST::AssignmentOperator_BitwiseXor ) == "X ^= Y" );
    CHECK( PrintAssignmentExpression( AST::AssignmentOperator_LeftShift ) == "X <<= Y" );
    CHECK( PrintAssignmentExpression( AST::AssignmentOperator_RightShift ) == "X >>= Y" );
}