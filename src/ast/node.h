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

#endif