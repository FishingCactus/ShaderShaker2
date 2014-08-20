#ifndef EXPRESSION_NODE_H
    #define EXPRESSION_NODE_H

    #include <ostream>
    #include <ast/assignment_operator.h>

    namespace AST
    {
        enum SelfModifyOperator
        {
            SelfModifyOperator_None,
            SelfModifyOperator_PlusPlus,
            SelfModifyOperator_MinusMinus
        };

        std::ostream& operator<<(
            std::ostream &out,
            SelfModifyOperator self_modify_operator
            );

        struct Expression : Node
        {
            virtual Expression * Clone() const override { return 0; }

        };

        struct ConditionalExpression : Expression
        {
            AST_HandleVisitor()

            virtual ConditionalExpression * Clone() const override;

            Base::ObjectRef<Expression> m_Condition;
            Base::ObjectRef<Expression> m_IfTrue;
            Base::ObjectRef<Expression> m_IfFalse;

        };

        struct BinaryOperationExpression : Expression
        {
            AST_HandleVisitor()
            enum Operation
            {
                LogicalOr,
                LogicalAnd,
                BitwiseOr,
                BitwiseXor,
                BitwiseAnd,
                Equality,
                Difference,
                LessThan,
                GreaterThan,
                LessThanOrEqual,
                GreaterThanOrEqual,
                BitwiseLeftShift,
                BitwiseRightShift,
                Addition,
                Subtraction,
                Multiplication,
                Division,
                Modulo,
            };

            BinaryOperationExpression() {}
            BinaryOperationExpression(
                Operation operation,
                Expression * left_expression,
                Expression * right_expression
                ) :
                m_Operation( operation ),
                m_LeftExpression( left_expression ),
                m_RightExpression( right_expression )
            {}

            virtual BinaryOperationExpression * Clone() const override;

            Operation
                m_Operation;
            Base::ObjectRef<Expression>
                m_LeftExpression,
                m_RightExpression;

        };

        std::ostream& operator<<(
            std::ostream &out,
            BinaryOperationExpression::Operation operation
            );

        struct UnaryOperationExpression : Expression
        {
            AST_HandleVisitor()
            enum Operation
            {
                Plus,
                Minus,
                Not,
                BitwiseNot
            };

            UnaryOperationExpression() {}
            UnaryOperationExpression(
                Operation operation,
                Expression * expression
                ) :
                m_Operation( operation ),
                m_Expression( expression )
            {}

            virtual UnaryOperationExpression * Clone() const override;

            Operation
                m_Operation;
            Base::ObjectRef<Expression>
                m_Expression;

        };

        std::ostream& operator<<(
            std::ostream &out,
            UnaryOperationExpression::Operation operation
            );

        struct CastExpression: Expression
        {
            AST_HandleVisitor()
            CastExpression() {}
            CastExpression(
                Type * type,
                int array_size,
                Expression * expression
                ) :
                m_Type( type ),
                m_ArraySize( array_size ),
                m_Expression( expression )
            {

            }

            virtual CastExpression * Clone() const override;

            Base::ObjectRef<Type>
                m_Type;
            int
                m_ArraySize;
            Base::ObjectRef<Expression>
                m_Expression;

        };

        struct LiteralExpression : Expression
        {
            AST_HandleVisitor()
            enum Type
            {
                Int,
                Float,
                Bool
            };

            LiteralExpression(){}
            LiteralExpression( Type type, const std::string & value ) : m_Type( type ), m_Value( value )
            {

            }

            virtual LiteralExpression * Clone() const override;

            Type
                m_Type;
            std::string
                m_Value;
        };

        struct VariableExpression: Expression
        {
            AST_HandleVisitor()

            VariableExpression() {}
            VariableExpression( const std::string & name ) : m_Name( name ){}

            virtual VariableExpression * Clone() const override;

            std::string
                m_Name;
            Base::ObjectRef<Expression>
                m_SubscriptExpression;
        };

        struct PostfixExpression : Expression
        {
            AST_HandleVisitor()
            PostfixExpression() {}
            PostfixExpression( Expression *expression, PostfixSuffix * suffix ) : m_Expression( expression ), m_Suffix( suffix ){}

            virtual PostfixExpression * Clone() const override;

            Base::ObjectRef<Expression>
                m_Expression;
            Base::ObjectRef<PostfixSuffix>
                m_Suffix;
        };

        struct ArgumentExpressionList : Node
        {
            AST_HandleVisitor()

            void AddExpression( Expression * expression ){ m_ExpressionList.emplace_back( expression ); }

            virtual ArgumentExpressionList * Clone() const override;

            std::vector<Base::ObjectRef<Expression> >
                m_ExpressionList;
        };

        struct CallExpression : Expression
        {
            AST_HandleVisitor()

            CallExpression(){}
            CallExpression( const std::string & name, ArgumentExpressionList * list ) : m_Name( name ), m_ArgumentExpressionList( list ) {}

            virtual CallExpression * Clone() const override;

            std::string
                m_Name;
            Base::ObjectRef<ArgumentExpressionList>
                m_ArgumentExpressionList;

        };

        struct ConstructorExpression : Expression
        {
            AST_HandleVisitor()

            ConstructorExpression() {}
            ConstructorExpression( Type * type, ArgumentExpressionList * list ) : m_Type( type ), m_ArgumentExpressionList( list ) {}

            virtual ConstructorExpression * Clone() const override;

            Base::ObjectRef<Type>
                m_Type;
            Base::ObjectRef<ArgumentExpressionList>
                m_ArgumentExpressionList;
        };

        struct PostfixSuffix : Node
        {
            virtual PostfixSuffix * Clone() const override { return 0; }
        };

        struct Swizzle : PostfixSuffix
        {
            AST_HandleVisitor()
            Swizzle(){}
            Swizzle( const std::string & swizzle ) : m_Swizzle( swizzle ) {}

            virtual Swizzle * Clone() const override;

            std::string
                m_Swizzle;
        };

        struct PostfixSuffixCall :PostfixSuffix
        {
            AST_HandleVisitor()

            PostfixSuffixCall(){}
            PostfixSuffixCall( CallExpression * call, PostfixSuffix * suffix ) : m_CallExpression( call ), m_Suffix( suffix ) {}

            virtual PostfixSuffixCall * Clone() const override;

            Base::ObjectRef<CallExpression>
                m_CallExpression;
            Base::ObjectRef<PostfixSuffix>
                m_Suffix;
        };

        struct PostfixSuffixVariable :PostfixSuffix
        {
            AST_HandleVisitor()

            PostfixSuffixVariable() {}
            PostfixSuffixVariable( VariableExpression * variable, PostfixSuffix * suffix )
                : m_VariableExpression( variable ), m_Suffix( suffix ) {}

            virtual PostfixSuffixVariable * Clone() const override;

            Base::ObjectRef<VariableExpression>
                m_VariableExpression;
            Base::ObjectRef<PostfixSuffix>
                m_Suffix;
        };

        struct LValueExpression : Expression
        {
            AST_HandleVisitor()

            LValueExpression() {}
            LValueExpression( VariableExpression * variable, PostfixSuffix * suffix = 0 )
                : m_VariableExpression( variable ), m_Suffix( suffix ) {}

            virtual LValueExpression * Clone() const override;

            Base::ObjectRef<VariableExpression>
                m_VariableExpression;
            Base::ObjectRef<PostfixSuffix>
                m_Suffix;
        };

        struct PreModifyExpression : Expression
        {
            AST_HandleVisitor()

            PreModifyExpression() {}
            PreModifyExpression( const SelfModifyOperator op, LValueExpression * expression ) : m_Operator( op ), m_Expression( expression ) {}

            virtual PreModifyExpression * Clone() const override;

            SelfModifyOperator
                m_Operator;
            Base::ObjectRef<LValueExpression>
                m_Expression;
        };

        struct PostModifyExpression : Expression
        {
            AST_HandleVisitor()

            PostModifyExpression() {}
            PostModifyExpression( const SelfModifyOperator op, LValueExpression * expression ) : m_Operator( op ), m_Expression( expression ) {}

            virtual PostModifyExpression * Clone() const override;

            SelfModifyOperator
                m_Operator;
            Base::ObjectRef<LValueExpression>
                m_Expression;
        };

        std::ostream& operator<<(
            std::ostream &out,
            AssignmentOperator assignment_operator
            );

        struct AssignmentExpression : Expression
        {
            AST_HandleVisitor()

            AssignmentExpression() {}
            AssignmentExpression( LValueExpression * lvexp, AssignmentOperator op, Expression * exp ) :
                m_LValueExpression( lvexp ), m_Operator( op ), m_Expression( exp )
            {

            }

            virtual AssignmentExpression * Clone() const override;

            Base::ObjectRef<LValueExpression>
                m_LValueExpression;
            AssignmentOperator
                m_Operator;
            Base::ObjectRef<Expression>
                m_Expression;
        };
    }
#endif