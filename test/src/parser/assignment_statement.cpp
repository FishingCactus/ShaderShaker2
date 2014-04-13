#include "catch.hpp"
#include "ast/node.h"
#include "parser_helper.h"

static AST::AssignmentOperator ParseOperator(
    const char * code
    )
{
    Parser parser( code, strlen( code ) );

    return parser.m_Parser.assignment_operator();
}

TEST_CASE( "Assignment operator are parsed", "[parser]" )
{
    CHECK( AST::AssignmentOperator_Assign == ParseOperator( " = " ) );
    CHECK( AST::AssignmentOperator_Multiply == ParseOperator( " *= " ) );
    CHECK( AST::AssignmentOperator_Divide == ParseOperator( " /= " ) );
    CHECK( AST::AssignmentOperator_Add == ParseOperator( " += " ) );
    CHECK( AST::AssignmentOperator_Subtract == ParseOperator( " -= " ) );
    CHECK( AST::AssignmentOperator_BitwiseAnd == ParseOperator( " &= " ) );
    CHECK( AST::AssignmentOperator_BitwiseOr == ParseOperator( " |= " ) );
    CHECK( AST::AssignmentOperator_BitwiseXor == ParseOperator( " ^= " ) );
    CHECK( AST::AssignmentOperator_LeftShift == ParseOperator( " <<= " ) );
    CHECK( AST::AssignmentOperator_RightShift == ParseOperator( " >>= " ) );
}