#pragma once

#include <type_traits>

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

    template<bool is_const>
    class VisitorBase
    {
        struct identity{ template<typename T> struct m{typedef T type;}; };
        struct add_const{ template<typename T> using m = std::add_const<T>; };
        using modifier = typename std::conditional<is_const, add_const, identity>::type;

    public:
        virtual void Visit( typename modifier::template m<Node>::type & node ) = 0;
        virtual void Visit( typename modifier::template m<TranslationUnit>::type & translation_unit ) = 0;
        virtual void Visit( typename modifier::template m<VariableDeclaration>::type & variable_declaration ) = 0;
        virtual void Visit( typename modifier::template m<IntrinsicType>::type & type ) = 0;
        virtual void Visit( typename modifier::template m<UserDefinedType>::type & type ) = 0;
        virtual void Visit( typename modifier::template m<SamplerType>::type & type ) = 0;
        virtual void Visit( typename modifier::template m<TypeModifier>::type & modifier ) = 0;
        virtual void Visit( typename modifier::template m<StorageClass>::type & storage_class ) = 0;
        virtual void Visit( typename modifier::template m<VariableDeclarationBody>::type & body ) = 0;
        virtual void Visit( typename modifier::template m<TextureDeclaration>::type & declaration ) = 0;
        virtual void Visit( typename modifier::template m<SamplerDeclaration>::type & declaration ) = 0;
        virtual void Visit( typename modifier::template m<SamplerBody>::type & body ) = 0;
        virtual void Visit( typename modifier::template m<StructDefinition>::type & definition ) = 0;
        virtual void Visit( typename modifier::template m<FunctionDeclaration>::type & function_declaration ) = 0;
        virtual void Visit( typename modifier::template m<ArgumentList>::type & argument_list ) = 0;
        virtual void Visit( typename modifier::template m<Argument>::type & argument ) = 0;

        // Expressions
        virtual void Visit( typename modifier::template m<LiteralExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<VariableExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<UnaryOperationExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<BinaryOperationExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<CallExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<ArgumentExpressionList>::type & list ) = 0;
        virtual void Visit( typename modifier::template m<Swizzle>::type & swizzle ) = 0;
        virtual void Visit( typename modifier::template m<PostfixSuffixCall>::type & postfix_suffix ) = 0;
        virtual void Visit( typename modifier::template m<PostfixSuffixVariable>::type & postfix_suffix ) = 0;
        virtual void Visit( typename modifier::template m<ConstructorExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<ConditionalExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<LValueExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<PreModifyExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<PostModifyExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<CastExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<AssignmentExpression>::type & expression ) = 0;
        virtual void Visit( typename modifier::template m<PostfixExpression>::type & expression ) = 0;

        // Statements

        virtual void Visit( typename modifier::template m<ReturnStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<BreakStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<ContinueStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<DiscardStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<EmptyStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<ExpressionStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<IfStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<WhileStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<DoWhileStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<BlockStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<AssignmentStatement>::type & statement ) = 0;
        virtual void Visit( typename modifier::template m<VariableDeclarationStatement>::type & statement ) = 0;

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
