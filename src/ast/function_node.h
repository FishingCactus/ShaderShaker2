#ifndef FUNCTION_NODE_H
    #define FUNCTION_NODE_H

    namespace AST
    {
        struct Statement;
        struct ArgumentList;
        struct Argument;

        struct FunctionDeclaration : GlobalDeclaration
        {
            AST_HandleVisitor()

            void AddStorageClass( StorageClass * storage ){ m_StorageClassTable.emplace_back( storage ); }
            void AddStatement( Statement * statement ){ m_StatementTable.emplace_back( statement ); }

            std::shared_ptr<Type> 
                m_Type;
            std::string 
                m_Name,
                m_Semantic;
            std::shared_ptr<ArgumentList> 
                m_ArgumentList;
            std::vector< std::shared_ptr<StorageClass> >
                m_StorageClassTable;
            std::vector< std::shared_ptr<Statement> >
                m_StatementTable;

        };

        struct ArgumentList : Node
        {
            AST_HandleVisitor()


            void AddArgument( Argument * argument ){ m_ArgumentTable.emplace_back( argument ); }

            std::vector< std::shared_ptr<Argument> >
                m_ArgumentTable;
        };

        struct Argument : Node
        {
            AST_HandleVisitor()
        };
    }

#endif