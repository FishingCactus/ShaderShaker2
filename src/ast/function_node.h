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

            virtual FunctionDeclaration * Clone() const override;

            Base::ObjectRef<Type>
                m_Type;
            std::string
                m_Name,
                m_Semantic;
            Base::ObjectRef<ArgumentList>
                m_ArgumentList;
            std::vector< Base::ObjectRef<StorageClass> >
                m_StorageClassTable;
            std::vector< Base::ObjectRef<Statement> >
                m_StatementTable;

        };

        struct ArgumentList : Node
        {
            AST_HandleVisitor()

            void AddArgument( Argument * argument ){ m_ArgumentTable.emplace_back( argument ); }

            virtual ArgumentList * Clone() const override;

            std::vector< Base::ObjectRef<Argument> >
                m_ArgumentTable;
        };

        struct Argument : Node
        {
            AST_HandleVisitor()

            virtual Argument * Clone() const override;

            Base::ObjectRef<Type>
                m_Type;
            std::string
                m_Name,
                m_InputModifier,
                m_Semantic,
                m_InterpolationModifier;
            Base::ObjectRef<TypeModifier>
                m_TypeModifier;
            Base::ObjectRef<InitialValue>
                m_InitialValue;
        };
    }

#endif