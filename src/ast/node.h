#ifndef NODE_H
    #define NODE_H

    #include <vector>
    #include <string>
    #include <cassert>

    #include "visitor.h"

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
            virtual void Visit( AST::Visitor & visitor ) override { visitor.Visit( *this ); }

        struct Node
        {
            virtual void Visit( AST::Visitor & visitor ){ visitor.Visit( *this ); };
            virtual ~Node(){}

        };

        struct TranslationUnit : Node
        {
            AST_HandleVisitor()
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

        struct TextureDeclaration : GlobalDeclaration
        {
            AST_HandleVisitor()
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

            std::string
                m_Type,
                m_Name,
                m_Semantic;
            std::shared_ptr<Annotations>
                m_Annotations;
        };

        struct SamplerDeclaration : GlobalDeclaration
        {
            AST_HandleVisitor()
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

            std::string
                m_Type,
                m_Name;
            std::vector< std::shared_ptr<SamplerBody> >
                m_BodyTable;
        };

        struct SamplerBody : Node
        {
            AST_HandleVisitor()
            SamplerBody(
                const std::string & name,
                const std::string & value
                ) :
                m_Name( name ),
                m_Value( value )
            {

            }

            std::string
                m_Name,
                m_Value;
        };

        struct StructDefinition: GlobalDeclaration
        {
            AST_HandleVisitor()
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

                std::shared_ptr<Type>
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

            std::string
                m_Name;
            std::vector<Member>
                m_MemberTable;

        };

        struct Type : Node
        {
            AST_HandleVisitor()
            Type( const std::string & name ) : m_Name( name ) {}

            std::string
                m_Name;
        };

        struct IntrinsicType : Type
        {
            AST_HandleVisitor()
            IntrinsicType( const std::string & name ) : Type( name ) {}
        };

        struct UserDefinedType : Type
        {
            AST_HandleVisitor()
            UserDefinedType( const std::string & name ) : Type( name ) {}
        };

        struct SamplerType : Type
        {
            SamplerType( const std::string & name ) : Type( name ) {}
        };

        struct TypeModifier : Node
        {
            AST_HandleVisitor()
            TypeModifier( const std::string & modifier ) : m_Value( modifier ){}
            std::string m_Value;
        };

        struct StorageClass : Node
        {
            AST_HandleVisitor()
            StorageClass( const std::string & storage_class ) : m_Value( storage_class ){}
            std::string m_Value;
        };

        struct VariableDeclarationBody : Node
        {
            AST_HandleVisitor()

            VariableDeclarationBody() : m_ArraySize( 0 ) {}

            std::string
                m_Name,
                m_Semantic;
            std::shared_ptr<InitialValue>
                m_InitialValue;
            int
                m_ArraySize;
            std::shared_ptr<Annotations>
                m_Annotations;

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

        struct PostfixExpression : Expression
        {
            AST_HandleVisitor()
            PostfixExpression( Expression *expression, PostfixSuffix * suffix ) : m_Expression( expression ), m_Suffix( suffix ){}

            std::shared_ptr<Expression>
                m_Expression;
            std::shared_ptr<PostfixSuffix>
                m_Suffix;
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

        struct InitialValue : Node
        {
            AST_HandleVisitor()

            InitialValue() : m_Vector( false ){}

            void AddExpression( Expression * expression ){ assert( expression ); m_ExpressionTable.emplace_back( expression ); }

            std::vector<std::shared_ptr<Expression> >
                m_ExpressionTable;
            bool
                m_Vector;
        };

        struct Technique : Node
        {

        };

    }

    #include "annotation_node.h"

#endif