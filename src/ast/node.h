#ifndef NODE_H
    #define NODE_H

    #include <vector>
    #include <string>
    #include <cassert>
    #include <memory>

    #include "visitor.h"
    #include "const_visitor.h"
    #include "base/object.h"
    #include "base/object_ref.h"

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
        struct VariableDeclarationBody;
        struct Expression;
        struct InitialValue;
        struct PostfixSuffix;
        struct Annotations;
        struct SamplerBody;

        #define AST_HandleVisitor() \
            virtual void Visit( AST::Visitor & visitor ) override { visitor.Visit( *this ); } \
            virtual void Visit( AST::ConstVisitor & visitor ) const override { visitor.Visit( *this ); }

        struct Node : public Base::Object
        {
            Node();
            virtual void Visit( AST::Visitor & visitor ){ visitor.Visit( *this ); };
            virtual void Visit( AST::ConstVisitor & visitor ) const { visitor.Visit( *this ); };

            virtual ~Node(){}

            virtual Node * Clone() const = 0;

            static void SetDebugInfo(
                const std::string & filename,
                const int line
                );

            static const std::string & GetCurrentFileName()
            {
                return s_CurrentFileName;
            }

            static int GetCurrentLine()
            {
                return s_CurrentLine;
            }

            const std::string
                m_FileName;
            const int
                m_Line;

        private:

            Node & operator =( const Node & );

            static std::string
                s_CurrentFileName;
            static int
                s_CurrentLine;
        };

        struct GlobalDeclaration : Node
        {
            virtual GlobalDeclaration * Clone() const override { return 0; }
        };

        struct TranslationUnit : Node
        {
            AST_HandleVisitor()

            void AddGlobalDeclaration( GlobalDeclaration * declaration )
            {
                assert( declaration );
                m_GlobalDeclarationTable.push_back( declaration );
            }

            void AddTechnique( Technique * technique )
            {
                assert( technique );
                m_TechniqueTable.emplace_back( technique );
            }

            virtual TranslationUnit * Clone() const override;

            std::vector< Base::ObjectRef<GlobalDeclaration> >
                m_GlobalDeclarationTable;
            std::vector< Base::ObjectRef<Technique> >
                m_TechniqueTable;
        };

        struct VariableDeclaration : GlobalDeclaration
        {
            AST_HandleVisitor()
            void SetType( Type * type ){ assert( type ); m_Type = type; }
            void AddStorageClass( StorageClass * storage_class ){ assert( storage_class ); m_StorageClass.emplace_back( storage_class ); }
            void AddTypeModifier( TypeModifier * type_modifier ){ assert( type_modifier ); m_TypeModifier.emplace_back( type_modifier ); }
            void AddBody( VariableDeclarationBody * body ){ assert( body ); m_BodyTable.emplace_back( body ); }

            virtual VariableDeclaration * Clone() const override;

            Base::ObjectRef<Type> m_Type;
            std::vector<Base::ObjectRef<StorageClass> > m_StorageClass;
            std::vector<Base::ObjectRef<TypeModifier> > m_TypeModifier;
            std::vector<Base::ObjectRef<VariableDeclarationBody> > m_BodyTable;
        };

        struct TextureDeclaration : GlobalDeclaration
        {
            AST_HandleVisitor()

            TextureDeclaration() {}
            TextureDeclaration(
                const std::string & type,
                const std::string & name,
                const std::string & semantic,
                Annotations * annotations
                ) :
                m_Type( type ),
                m_Name( name ),
                m_Semantic( semantic ),
                m_Annotations( annotations )
            {

            }

            virtual TextureDeclaration * Clone() const override;

            std::string
                m_Type,
                m_Name,
                m_Semantic;
            Base::ObjectRef<Annotations>
                m_Annotations;
        };

        struct SamplerDeclaration : GlobalDeclaration
        {
            AST_HandleVisitor()

            SamplerDeclaration() {}
            SamplerDeclaration(
                const std::string & type,
                const std::string & name
                ) :
                m_Type( type ),
                m_Name( name )
            {

            }

            void AddBody( SamplerBody * body )
            {
                m_BodyTable.emplace_back( body );
            }

            virtual SamplerDeclaration * Clone() const override;


            std::string
                m_Type,
                m_Name;
            std::vector< Base::ObjectRef<SamplerBody> >
                m_BodyTable;
        };

        struct SamplerBody : Node
        {
            AST_HandleVisitor()
            SamplerBody(){}
            SamplerBody(
                const std::string & name,
                const std::string & value
                ) :
                m_Name( name ),
                m_Value( value )
            {

            }

            virtual SamplerBody * Clone() const override;

            std::string
                m_Name,
                m_Value;
        };

        struct StructDefinition: GlobalDeclaration
        {
            AST_HandleVisitor()
            StructDefinition() {}
            StructDefinition( const std::string & name ) : m_Name( name ) {}

            struct Member
            {
                Member(
                    Type * type,
                    const std::string & name,
                    const std::string & semantic,
                    const std::string & interpolation_modifier
                    ) :
                    m_Type( type ),
                    m_Name( name ),
                    m_Semantic( semantic ),
                    m_InterpolationModifier( interpolation_modifier )
                {

                }

                Base::ObjectRef<Type>
                    m_Type;
                std::string
                    m_Name,
                    m_Semantic,
                    m_InterpolationModifier;
            };

            void AddMember(
                const std::string & name,
                Type * type,
                const std::string & semantic,
                const std::string & interpolation_modifier
                )
            {
                // :HACK: Visual studio 2010 does not support mutliple argument emplace_back, replace when fully C++11 compliant
                m_MemberTable.emplace_back( Member( type, name, semantic, interpolation_modifier ) );
            }

            virtual StructDefinition * Clone() const override;

            std::string
                m_Name;
            std::vector<Member>
                m_MemberTable;

        };

        struct Type : Node
        {
            AST_HandleVisitor()

            Type() {}
            Type( const std::string & name ) : m_Name( name ) {}

            virtual Type * Clone() const override { return 0; }

            std::string
                m_Name;
        };

        struct IntrinsicType : Type
        {
            AST_HandleVisitor()

            IntrinsicType(){}
            IntrinsicType( const std::string & name ) : Type( name ) {}

            virtual IntrinsicType * Clone() const override;
        };

        struct UserDefinedType : Type
        {
            AST_HandleVisitor()

            UserDefinedType() {}
            UserDefinedType( const std::string & name ) : Type( name ) {}

            virtual UserDefinedType * Clone() const override;
        };

        struct SamplerType : Type
        {
            SamplerType() {}
            SamplerType( const std::string & name ) : Type( name ) {}

            virtual SamplerType * Clone() const override;
        };

        struct TypeModifier : Node
        {
            AST_HandleVisitor()

            TypeModifier() {}
            TypeModifier( const std::string & modifier ) : m_Value( modifier ){}

            virtual TypeModifier * Clone() const override;
            std::string m_Value;
        };

        struct StorageClass : Node
        {
            AST_HandleVisitor()

            StorageClass(){}
            StorageClass( const std::string & storage_class ) : m_Value( storage_class ){}
            virtual StorageClass * Clone() const override;
            std::string m_Value;
        };

        struct VariableDeclarationBody : Node
        {
            AST_HandleVisitor()

            VariableDeclarationBody() : m_ArraySize( -1 ) {}
            VariableDeclarationBody( const std::string & name ) : m_Name( name ), m_ArraySize( -1 ) {}

            virtual VariableDeclarationBody * Clone() const override;

            std::string
                m_Name,
                m_Semantic;
            Base::ObjectRef<InitialValue>
                m_InitialValue;
            int
                m_ArraySize;
            Base::ObjectRef<Annotations>
                m_Annotations;
        };

        struct InitialValue : Node
        {
            AST_HandleVisitor()

            InitialValue() : m_Vector( false ){}

            void AddExpression( Expression * expression ){ assert( expression ); m_ExpressionTable.emplace_back( expression ); }

            virtual InitialValue * Clone() const override;

            std::vector<Base::ObjectRef<Expression> >
                m_ExpressionTable;
            bool
                m_Vector;
        };

    }

    #include "expression_node.h"
    #include "annotation_node.h"
    #include "function_node.h"
    #include "statement_node.h"
    #include "technique_node.h"

#endif