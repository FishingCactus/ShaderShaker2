#ifndef EXPRESSION_NODE_H
    #define EXPRESSION_NODE_H

    namespace AST
    {
        enum SelfModifyOperator
        {
            SelfModifyOperator_None,
            SelfModifyOperator_PlusPlus,
            SelfModifyOperator_MinusMinus
        };

        struct Expression : Node
        {

        };

        struct ConditionalExpression : Expression
        {
            AST_HandleVisitor()

            std::shared_ptr<Expression> m_Condition;
            std::shared_ptr<Expression> m_IfTrue;
            std::shared_ptr<Expression> m_IfFalse;

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

            BinaryOperationExpression(
                Operation operation,
                Expression * left_expression,
                Expression * right_expression
                ) :
                m_Operation( operation ),
                m_LeftExpression( left_expression ),
                m_RightExpression( right_expression )
            {}

            Operation
                m_Operation;
            std::shared_ptr<Expression>
                m_LeftExpression,
                m_RightExpression;

        };

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

            UnaryOperationExpression(
                Operation operation,
                Expression * expression
                ) :
                m_Operation( operation ),
                m_Expression( expression )
            {}

            Operation
                m_Operation;
            std::shared_ptr<Expression>
                m_Expression;

        };

        struct CastExpression: Expression
        {
            AST_HandleVisitor()
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

            std::shared_ptr<Type>
                m_Type;
            int
                m_ArraySize;
            std::shared_ptr<Expression>
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

            LiteralExpression( Type type, const std::string & value ) : m_Type( type ), m_Value( value )
            {

            }

            Type
                m_Type;
            std::string
                m_Value;
        };

        struct VariableExpression: Expression
        {
            AST_HandleVisitor()

            VariableExpression( const std::string & name ) : m_Name( name ){}

            std::string
                m_Name;
            std::shared_ptr<Expression>
                m_SubscriptExpression;
        };

        struct PostfixExpression : Expression
        {
            AST_HandleVisitor()
            PostfixExpression( Expression *expression, PostfixSuffix * suffix ) : m_Expression( expression ), m_Suffix( suffix ){}

            std::shared_ptr<Expression>
                m_Expression;
            std::shared_ptr<PostfixSuffix>
                m_Suffix;
        };

        struct ArgumentExpressionList : Node
        {
            AST_HandleVisitor()

            void AddExpression( Expression * expression ){ m_ExpressionList.emplace_back( expression ); }

            std::vector<std::shared_ptr<Expression> >
                m_ExpressionList;
        };

        struct CallExpression : Expression
        {
            AST_HandleVisitor()

            CallExpression( const std::string & name, ArgumentExpressionList * list ) : m_Name( name ), m_ArgumentExpressionList( list ) {}

            std::string
                m_Name;
            std::shared_ptr<ArgumentExpressionList>
                m_ArgumentExpressionList;

        };

        struct ConstructorExpression : Expression
        {
            AST_HandleVisitor()

            ConstructorExpression( Type * type, ArgumentExpressionList * list ) : m_Type( type ), m_ArgumentExpressionList( list ) {}

            std::shared_ptr<Type>
                m_Type;
            std::shared_ptr<ArgumentExpressionList>
                m_ArgumentExpressionList;
        };

        struct PostfixSuffix : Node
        {

        };

        struct Swizzle : PostfixSuffix
        {
            AST_HandleVisitor()
            Swizzle( const std::string & swizzle ) : m_Swizzle( swizzle ) {}

            std::string
                m_Swizzle;
        };

        struct PostfixSuffixCall :PostfixSuffix
        {
            AST_HandleVisitor()

            PostfixSuffixCall( CallExpression * call, PostfixSuffix * suffix ) : m_CallExpression( call ), m_Suffix( suffix ) {}

            std::shared_ptr<CallExpression>
                m_CallExpression;
            std::shared_ptr<PostfixSuffix>
                m_Suffix;
        };

        struct PostfixSuffixVariable :PostfixSuffix
        {
            AST_HandleVisitor()

            PostfixSuffixVariable( VariableExpression * variable, PostfixSuffix * suffix ) : m_VariableExpression( variable ), m_Suffix( suffix ) {}

            std::shared_ptr<VariableExpression>
                m_VariableExpression;
            std::shared_ptr<PostfixSuffix>
                m_Suffix;
        };

        struct LValueExpression : Expression
        {
            LValueExpression( VariableExpression * variable, PostfixSuffix * suffix ) : m_VariableExpression( variable ), m_Suffix( suffix ) {}

            std::shared_ptr<VariableExpression>
                m_VariableExpression;
            std::shared_ptr<PostfixSuffix>
                m_Suffix;
        };

        struct PreModifyExpression : Expression
        {
            PreModifyExpression( const SelfModifyOperator op, LValueExpression * expression ) : m_Operator( op ), m_Expression( expression ) {}

            SelfModifyOperator
                m_Operator;
            std::shared_ptr<LValueExpression>
                m_Expression;
        };
    }
#endif