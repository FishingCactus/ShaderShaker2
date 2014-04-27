#ifndef STATEMENT_NODE_H
    #define STATEMENT_NODE_H

    namespace AST
    {
        struct Expression;

        struct Statement : Node
        {

        };

        struct ReturnStatement : Statement
        {
            AST_HandleVisitor()

            ReturnStatement(){}
            ReturnStatement( Expression * expression ) : m_Expression( expression ) {}

            std::shared_ptr<Expression>
                m_Expression;
        };

        struct BreakStatement : Statement
        {
            AST_HandleVisitor()
        };

        struct ContinueStatement : Statement
        {
            AST_HandleVisitor()
        };

        struct DiscardStatement : Statement
        {
            AST_HandleVisitor()
        };

        struct EmptyStatement : Statement
        {
            AST_HandleVisitor()
        };

        struct ExpressionStatement : Statement
        {
            AST_HandleVisitor()

            ExpressionStatement( Expression * expression ) : m_Expression( expression ) {}

            std::shared_ptr<Expression>
                m_Expression;
        };

        struct IfStatement : Statement
        {
            AST_HandleVisitor()

            IfStatement( Expression * condition, Statement * then_statement, Statement * else_statement )
                :
                m_Condition( condition ),
                m_ThenStatement( then_statement ),
                m_ElseStatement( else_statement )
            {

            }

            std::shared_ptr<Expression>
                m_Condition;
            std::shared_ptr<Statement>
                m_ThenStatement,
                m_ElseStatement;
        };

        struct WhileStatement : Statement
        {
            WhileStatement( Expression * condition, Statement * statement ) : m_Condition( condition ), m_Statement( statement ) {}

            std::shared_ptr<Expression>
                m_Condition;
            std::shared_ptr<Statement>
                m_Statement;
        };

        struct DoWhileStatement : Statement
        {
            DoWhileStatement( Expression * condition, Statement * statement ) : m_Condition( condition ), m_Statement( statement ) {}

            std::shared_ptr<Expression>
                m_Condition;
            std::shared_ptr<Statement>
                m_Statement;
        };

        struct ForStatement : Statement
        {
            ForStatement( Statement * init_statement, Expression * equality_expression, Expression * modify_expression, Statement * statement ) :
                m_InitStatement( init_statement ), m_Statement( statement ),
                m_EqualityExpression( equality_expression ), m_ModifyExpression( modify_expression ){}

            std::shared_ptr<Statement>
                m_InitStatement,
                m_Statement;
            std::shared_ptr<Expression>
                m_EqualityExpression,
                m_ModifyExpression;
        };

        struct BlockStatement : Statement
        {
            void AddStatement( Statement * statement ){ m_StatementTable.emplace_back( statement ); }

            std::vector< std::shared_ptr<Statement> >
                m_StatementTable;
        };

        struct VariableDeclarationStatement : Statement
        {
            AST_HandleVisitor()

            void SetType( Type * type ){ assert( type ); m_Type.reset( type ); }
            void AddStorageClass( StorageClass * storage_class ){ assert( storage_class ); m_StorageClass.emplace_back( storage_class ); }
            void AddTypeModifier( TypeModifier * type_modifier ){ assert( type_modifier ); m_TypeModifier.emplace_back( type_modifier ); }
            void AddBody( VariableDeclarationBody * body ){ assert( body ); m_BodyTable.emplace_back( body ); }

            std::shared_ptr<Type> m_Type;
            std::vector<std::shared_ptr<StorageClass> > m_StorageClass;
            std::vector<std::shared_ptr<TypeModifier> > m_TypeModifier;
            std::vector<std::shared_ptr<VariableDeclarationBody> > m_BodyTable;
        };

        struct AssignmentStatement : Statement
        {
            AssignmentStatement( LValueExpression * lvexp, AssignmentOperator op, Expression * exp ) :
                m_Expression( new AssignmentExpression( lvexp, op, exp ) )
            {

            }

            std::shared_ptr<AssignmentExpression>
                m_Expression;
        };

    }

#endif