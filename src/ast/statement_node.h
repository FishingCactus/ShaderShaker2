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

    }

#endif