#ifndef TECHNIQUE_NODE_H
    #define TECHNIQUE_NODE_H


    namespace AST
    {

        struct Pass;
        struct ShaderDefinition;
        struct ShaderArgumentList;

        struct Technique : Node
        {
            Technique( const std::string & name ) : m_Name( name ) {}

            void AddPass( Pass * pass )
            {
                assert( pass );
                m_PassTable.emplace_back( pass );
            }

            std::string
                m_Name;
            std::vector< Base::ObjectRef<Pass> >
                m_PassTable;
        };

        struct Pass : Node
        {
            Pass( const std::string & name ) : m_Name( name ) {}

            void AddShaderDefinition( ShaderDefinition * definition )
            {
                assert( definition );
                m_ShaderDefinitionTable.emplace_back( definition );
            }

            std::string
                m_Name;
            std::vector< Base::ObjectRef<ShaderDefinition> >
                m_ShaderDefinitionTable;
        };

        enum ShaderType
        {
            ShaderType_Vertex,
            ShaderType_Pixel
        };

        struct ShaderDefinition : Node
        {
            ShaderDefinition( ShaderType type, const std::string & name, ShaderArgumentList * list ) :
                m_Name( name ), m_Type( type ), m_List( list ) {}

            std::string
                m_Name;
            ShaderType
                m_Type;
            Base::ObjectRef<ShaderArgumentList>
                m_List;
        };

        struct ShaderArgumentList : Node
        {
            ShaderArgumentList( Expression * argument )
            {
                m_ShaderArgumentTable.emplace_back( argument );
            }

            void AddArgument( Expression * argument )
            {
                m_ShaderArgumentTable.emplace_back( argument );
            }

            std::vector< Base::ObjectRef<Expression> >
                m_ShaderArgumentTable;
        };
    }

#endif