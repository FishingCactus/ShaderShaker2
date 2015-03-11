#pragma once

#include <type_traits>

namespace AST
{
    template<bool is_const>
    class VisitorBase
    {
        struct identity{ template<typename T> struct m{typedef T type;}; };
        struct add_const{ template<typename T> using m = std::add_const<T>; };
        using modifier = typename std::conditional<is_const, add_const, identity>::type;

    public:
        virtual void Visit( typename modifier::template m< struct Node>::type & node ) = 0;
        virtual void Visit( typename modifier::template m< struct TranslationUnit>::type & translation_unit ) = 0;
        virtual void Visit( typename modifier::template m< struct VariableDeclaration>::type & variable_declaration ) = 0;
        virtual void Visit( typename modifier::template m< struct IntrinsicType>::type & type ) = 0;
        virtual void Visit( typename modifier::template m< struct UserDefinedType>::type & type ) = 0;
        virtual void Visit( typename modifier::template m< struct SamplerType>::type & type ) = 0;
        virtual void Visit( typename modifier::template m< struct TypeModifier>::type & modifier ) = 0;
        virtual void Visit( typename modifier::template m< struct StorageClass>::type & storage_class ) = 0;
        virtual void Visit( typename modifier::template m< struct VariableDeclarationBody>::type & body ) = 0;
        virtual void Visit( typename modifier::template m< struct InitialValue>::type & initial_value ) = 0;
        virtual void Visit( typename modifier::template m< struct Annotations>::type & annotations ) = 0;
        virtual void Visit( typename modifier::template m< struct AnnotationEntry>::type & annotation_entry ) = 0;
        virtual void Visit( typename modifier::template m< struct TextureDeclaration>::type & declaration ) = 0;
        virtual void Visit( typename modifier::template m< struct SamplerDeclaration>::type & declaration ) = 0;
        virtual void Visit( typename modifier::template m< struct SamplerBody>::type & body ) = 0;
        virtual void Visit( typename modifier::template m< struct StructDefinition>::type & definition ) = 0;
        virtual void Visit( typename modifier::template m< struct FunctionDeclaration>::type & function_declaration ) = 0;
        virtual void Visit( typename modifier::template m< struct ArgumentList>::type & argument_list ) = 0;
        virtual void Visit( typename modifier::template m< struct Argument>::type & argument ) = 0;

        // Expressions
        virtual void Visit( typename modifier::template m< struct LiteralExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct VariableExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct UnaryOperationExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct BinaryOperationExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct CallExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct ArgumentExpressionList>::type & list ) = 0;
        virtual void Visit( typename modifier::template m< struct Swizzle>::type & swizzle ) = 0;
        virtual void Visit( typename modifier::template m< struct PostfixSuffixCall>::type & postfix_suffix ) = 0;
        virtual void Visit( typename modifier::template m< struct PostfixSuffixVariable>::type & postfix_suffix ) = 0;
        virtual void Visit( typename modifier::template m< struct ConstructorExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct ConditionalExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct LValueExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct PreModifyExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct PostModifyExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct CastExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct AssignmentExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m< struct PostfixExpression>::type & expression ) = 0;

        // Statements

        virtual void Visit( typename modifier::template m< struct ReturnStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct BreakStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct ContinueStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct DiscardStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct EmptyStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct ExpressionStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct IfStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct WhileStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct DoWhileStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct BlockStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct AssignmentStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m< struct VariableDeclarationStatement>::type & statement ) = 0;

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
    };
}
