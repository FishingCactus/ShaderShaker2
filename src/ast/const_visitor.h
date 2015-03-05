#ifndef CONST_VISITOR_H
    #define CONST_VISITOR_H

    #include "visitor_item_separator.h"

    namespace AST
    {
        #define AST_EmptyVisitOverride( _type_ ) virtual void Visit( const _type_ & /*node*/ ) override {}

        class ConstVisitor
        {
        public:

            virtual void Visit( const struct Node & node ) = 0;
            virtual void Visit( const struct TranslationUnit & translation_unit ) = 0;
            virtual void Visit( const struct VariableDeclaration & variable_declaration ) = 0;
            virtual void Visit( const struct IntrinsicType & type ) = 0;
            virtual void Visit( const struct UserDefinedType & type ) = 0;
            virtual void Visit( const struct SamplerType & type ) = 0;
            virtual void Visit( const struct TypeModifier & modifier ) = 0;
            virtual void Visit( const struct StorageClass & storage_class ) = 0;
            virtual void Visit( const struct VariableDeclarationBody & body ) = 0;
            virtual void Visit( const struct InitialValue & initial_value ) = 0;
            virtual void Visit( const struct TextureDeclaration & declaration ) = 0;
            virtual void Visit( const struct SamplerDeclaration & declaration ) = 0;
            virtual void Visit( const struct SamplerBody & body ) = 0;
            virtual void Visit( const struct StructDefinition & definition ) = 0;
            virtual void Visit( const struct FunctionDeclaration & function_declaration ) = 0;
            virtual void Visit( const struct ArgumentList & argument_list ) = 0;
            virtual void Visit( const struct Argument & argument ) = 0;
            virtual void Visit( const struct Annotations & annotations ) = 0;
            virtual void Visit( const struct AnnotationEntry & annotation_entry ) = 0;

            // Expressions
            virtual void Visit( const struct LiteralExpression & expression ) = 0;
            virtual void Visit( const struct VariableExpression & expression ) = 0;
            virtual void Visit( const struct UnaryOperationExpression & expression ) = 0;
            virtual void Visit( const struct BinaryOperationExpression & expression ) = 0;
            virtual void Visit( const struct CallExpression & expression ) = 0;
            virtual void Visit( const struct ArgumentExpressionList & list ) = 0;
            virtual void Visit( const struct Swizzle & swizzle ) = 0;
            virtual void Visit( const struct PostfixSuffixCall & postfix_suffix ) = 0;
            virtual void Visit( const struct PostfixSuffixVariable & postfix_suffix ) = 0;
            virtual void Visit( const struct ConstructorExpression & expression ) = 0;
            virtual void Visit( const struct ConditionalExpression & expression ) = 0;
            virtual void Visit( const struct LValueExpression & expression ) = 0;
            virtual void Visit( const struct PreModifyExpression & expression ) = 0;
            virtual void Visit( const struct PostModifyExpression & expression ) = 0;
            virtual void Visit( const struct CastExpression & expression ) = 0;
            virtual void Visit( const struct AssignmentExpression & expression ) = 0;
            virtual void Visit( const struct PostfixExpression & expression ) = 0;

            // Statements
            virtual void Visit( const struct ReturnStatement & statement ) = 0;
            virtual void Visit( const struct BreakStatement & statement ) = 0;
            virtual void Visit( const struct ContinueStatement & statement ) = 0;
            virtual void Visit( const struct DiscardStatement & statement ) = 0;
            virtual void Visit( const struct EmptyStatement & statement ) = 0;
            virtual void Visit( const struct ExpressionStatement & statement ) = 0;
            virtual void Visit( const struct IfStatement & statement ) = 0;
            virtual void Visit( const struct WhileStatement & statement ) = 0;
            virtual void Visit( const struct DoWhileStatement & statement ) = 0;
            virtual void Visit( const struct BlockStatement & statement ) = 0;
            virtual void Visit( const struct AssignmentStatement & statement ) = 0;
            virtual void Visit( const struct VariableDeclarationStatement & statement ) = 0;
        };

        template< class _Table_, typename _Separator_ >
        void VisitTable( ConstVisitor & visitor, _Table_ & table, _Separator_ & separator )
        {
            typename _Table_::const_iterator
                it = table.cbegin(),
                end = table.cend();

            for( ; it != end; ++it )
            {
                separator( visitor, table, it );
            }
        }

        template< class _Table_ >
        void VisitTable( ConstVisitor & visitor, _Table_ & table )
        {
            typename _Table_::const_iterator
                it = table.cbegin(),
                end = table.cend();

            for ( ; it != end; ++it )
            {
                ( *it )->Visit( visitor );
            }
        }
    }

#endif