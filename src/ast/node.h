#ifndef NODE_H
    #define NODE_H

    #include <vector>

    namespace AST
    {
        struct GlobalDeclaration;
        struct VariableDeclaration;
        struct Type;
        struct IntrinsicType;
        struct UserDefinedType;
        struct SamplerType;
        struct Technique;
        struct TypeModifier;
        struct StorageClass;

        struct Node
        {
            virtual ~Node(){}

        };

        struct TranslationUnit : Node
        {
            void AddGlobalDeclaration( GlobalDeclaration * declaration )
            {
                assert( declaration );
                m_GlobalDeclarationTable.emplace_back( declaration );
            }

            std::vector< std::shared_ptr<GlobalDeclaration> >
                m_GlobalDeclarationTable;
            std::vector< std::shared_ptr<Technique> >
                m_TechniqueTable;
        };

        struct GlobalDeclaration : Node
        {

        };

        struct VariableDeclaration : GlobalDeclaration
        {
            void SetType( Type * type ){ assert( type ); m_Type.reset( type ); }
            void AddStorageClass( StorageClass * storage_class ){ assert( storage_class ); m_StorageClass.emplace_back( storage_class ); }
            void AddTypeModifier( TypeModifier * type_modifier ){ assert( type_modifier ); m_TypeModifier.emplace_back( type_modifier ); }

            std::shared_ptr<Type> m_Type;
            std::vector<std::shared_ptr<StorageClass> > m_StorageClass;
            std::vector<std::shared_ptr<TypeModifier> > m_TypeModifier;
        };

        struct Type : Node
        {
            Type( const std::string & name ) : m_Name( name ) {}

            std::string
                m_Name;
        };

        struct IntrinsicType : Type
        {
            IntrinsicType( const std::string & name ) : Type( name ) {}
        };

        struct UserDefinedType : Type
        {
            UserDefinedType( const std::string & name ) : Type( name ) {}
        };

        struct SamplerType : Type
        {
            SamplerType( const std::string & name ) : Type( name ) {}
        };

        struct TypeModifier : Node
        {
            TypeModifier( const std::string & modifier ) : m_Value( modifier ){}
            std::string m_Value;
        };

        struct StorageClass : Node
        {
            StorageClass( const std::string & storage_class ) : m_Value( storage_class ){}
            std::string m_Value;
        };

    }

#endif