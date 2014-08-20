#ifndef ASSIGNMENT_OPERATOR_H
	#define ASSIGNMENT_OPERATOR_H

    namespace AST
    {
        enum AssignmentOperator
        {
            AssignmentOperator_Assign = 0,
            AssignmentOperator_Multiply,
            AssignmentOperator_Divide,
            AssignmentOperator_Add,
            AssignmentOperator_Subtract,
            AssignmentOperator_BitwiseAnd,
            AssignmentOperator_BitwiseOr,
            AssignmentOperator_BitwiseXor,
            AssignmentOperator_LeftShift,
            AssignmentOperator_RightShift,
            AssignmentOperator_None = -1
        };
    }

#endif