#ifndef CONST_VISITOR_H
    #define CONST_VISITOR_H


    namespace AST
    {

        struct Node;
        struct TranslationUnit;
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
        struct InitialValue;
        struct Annotations;
        struct AnnotationEntry;
        struct TextureDeclaration;
        struct SamplerDeclaration;
        struct SamplerBody;
        struct StructDefinition;
        struct FunctionDeclaration;
        struct ArgumentList;
        struct Argument;

        struct LiteralExpression;
        struct VariableExpression;
        struct UnaryOperationExpression;
        struct BinaryOperationExpression;
        struct CallExpression;
        struct ArgumentExpressionList;
        struct Swizzle;
        struct PostfixSuffixCall;
        struct PostfixSuffixVariable;
        struct ConstructorExpression;
        struct ConditionalExpression;
        struct LValueExpression;
        struct PreModifyExpression;
        struct PostModifyExpression;
        struct CastExpression;
        struct AssignmentExpression;
        struct PostfixExpression;

        struct ReturnStatement;
        struct BreakStatement;
        struct ContinueStatement;
        struct DiscardStatement;
        struct EmptyStatement;
        struct ExpressionStatement;
        struct IfStatement;
        struct WhileStatement;
        struct DoWhileStatement;
        struct BlockStatement;
        struct AssignmentStatement;
        struct VariableDeclarationStatement;

        class ConstVisitor
        {

        public:
            virtual void Visit( const Node & node ) = 0;
            virtual void Visit( const TranslationUnit & translation_unit ) = 0;
            virtual void Visit( const VariableDeclaration & variable_declaration ) = 0;
            virtual void Visit( const Type & type ) = 0;
            virtual void Visit( const IntrinsicType & type ) { Visit( ( const Type & ) type ); }
            virtual void Visit( const UserDefinedType & type ) = 0;
            virtual void Visit( const SamplerType & type ) = 0;
            virtual void Visit( const TypeModifier & modifier ) = 0;
            virtual void Visit( const StorageClass & storage_class ) = 0;
            virtual void Visit( const VariableDeclarationBody & body ) = 0;
            virtual void Visit( const InitialValue & initial_value ) = 0;
            virtual void Visit( const Annotations & annotations ) = 0;
            virtual void Visit( const AnnotationEntry & annotation_entry ) = 0;
            virtual void Visit( const TextureDeclaration & declaration ) = 0;
            virtual void Visit( const SamplerDeclaration & declaration ) = 0;
            virtual void Visit( const SamplerBody & body ) = 0;
            virtual void Visit( const StructDefinition & definition ) = 0;
            virtual void Visit( const FunctionDeclaration & function_declaration ) = 0;
            virtual void Visit( const ArgumentList & argument_list ) = 0;
            virtual void Visit( const Argument & argument ) = 0;

            // Expressions
            virtual void Visit( const LiteralExpression & expression ) = 0;
            virtual void Visit( const VariableExpression & expression ) = 0;
            virtual void Visit( const UnaryOperationExpression & expression ) = 0;
            virtual void Visit( const BinaryOperationExpression & expression ) = 0;
            virtual void Visit( const CallExpression & expression ) = 0;
            virtual void Visit( const ArgumentExpressionList & list ) = 0;
            virtual void Visit( const Swizzle & swizzle ) = 0;
            virtual void Visit( const PostfixSuffixCall & postfix_suffix ) = 0;
            virtual void Visit( const PostfixSuffixVariable & postfix_suffix ) = 0;
            virtual void Visit( const ConstructorExpression & expression ) = 0;
            virtual void Visit( const ConditionalExpression & expression ) = 0;
            virtual void Visit( const LValueExpression & expression ) = 0;
            virtual void Visit( const PreModifyExpression & expression ) = 0;
            virtual void Visit( const PostModifyExpression & expression ) = 0;
            virtual void Visit( const CastExpression & expression ) = 0;
            virtual void Visit( const AssignmentExpression & expression ) = 0;
            virtual void Visit( const PostfixExpression & expression ) = 0;

            // Statements

            virtual void Visit( const ReturnStatement & statement ) = 0;
            virtual void Visit( const BreakStatement & statement ) = 0;
            virtual void Visit( const ContinueStatement & statement ) = 0;
            virtual void Visit( const DiscardStatement & statement ) = 0;
            virtual void Visit( const EmptyStatement & statement ) = 0;
            virtual void Visit( const ExpressionStatement & statement ) = 0;
            virtual void Visit( const IfStatement & statement ) = 0;
            virtual void Visit( const WhileStatement & statement ) = 0;
            virtual void Visit( const DoWhileStatement & statement ) = 0;
            virtual void Visit( const BlockStatement & statement ) = 0;
            virtual void Visit( const AssignmentStatement & statement ) = 0;
            virtual void Visit( const VariableDeclarationStatement & statement ) = 0;
        };

        template< class _Table_ >
        void VisitTable( ConstVisitor & visitor, _Table_ & table )
        {
            typename _Table_::const_iterator
                it = table.cbegin(),
                end = table.cend();

            for( ; it != end; ++it )
            {
                (*it)->Visit( visitor );
            }
        }
    }

#endif