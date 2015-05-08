#include "print_visitor.h"

#include <assert.h>
#include <iostream>
#include "ast/node.h"
#include "utils/indentation.h"

namespace AST
{
    void PrintVisitor::Visit( const Node & /*node*/ )
    {
        assert(! "Some node type might not be supported in the visitor" );

    }

    void PrintVisitor::Visit( const TranslationUnit & translation_unit )
    {
        std::cout
            << "TranslationUnit" << endl_ind
            << "{" << inc_ind << endl_ind;

        VisitTable( *this, translation_unit.m_GlobalDeclarationTable );
        VisitTable( *this, translation_unit.m_TechniqueTable );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const VariableDeclaration & variable_declaration )
    {
        std::cout
            << "VariableDeclaration" << endl_ind
            << "{" << inc_ind << endl_ind;

        variable_declaration.m_Type->Visit( *this );

        VisitTable( *this, variable_declaration.m_StorageClass );
        VisitTable( *this, variable_declaration.m_TypeModifier );

        VisitTable( *this, variable_declaration.m_BodyTable );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const IntrinsicType & type )
    {
        std::cout << "IntrinsicType{ " << type.m_Name << " }" << endl_ind;
    }

    void PrintVisitor::Visit( const UserDefinedType & type )
    {
        std::cout << "UserDefinedType{ " << type.m_Name << " }" << endl_ind;
    }

    void PrintVisitor::Visit( const SamplerType & type )
    {
        std::cout << "SamplerType{ " << type.m_Name << " }" << endl_ind;
    }

    void PrintVisitor::Visit( const TypeModifier & modifier )
    {
        std::cout << "TypeModifier{ " << modifier.m_Value << " }" << endl_ind;

    }

    void PrintVisitor::Visit( const StorageClass & storage_class )
    {
        std::cout << "StorageClass{ " << storage_class.m_Value << " }" << endl_ind;
    }

    void PrintVisitor::Visit( const VariableDeclarationBody & body )
    {

        std::cout
            << "VariableDeclarationBody" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << body.m_Name << " }" << endl_ind;

        if( !body.m_Semantic.empty() )
        {
            std::cout << "Semantic{ " << body.m_Semantic << " }" << endl_ind;
        }

        if( body.m_Annotations )
        {
            body.m_Annotations->Visit( *this );
        }

        if( body.m_InitialValue )
        {
            body.m_InitialValue->Visit( *this );
        }

        if( body.m_ArraySize != -1 )
        {
            std::cout << "ArraySize{ " << body.m_ArraySize << " }" << endl_ind;
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;

    }

    void PrintVisitor::Visit( const InitialValue & initial_value )
    {
        std::cout << "InitialValue{ ";
        VisitTable( *this, initial_value.m_ExpressionTable );
        std::cout << " }" << endl_ind;
    }

    void PrintVisitor::Visit( const Annotations & annotations )
    {
        std::cout << "Annotations" << endl_ind
            << "{ " << inc_ind << endl_ind;

        VisitTable( *this, annotations.m_AnnotationTable );

        std::cout << dec_ind  << endl_ind << " }" << endl_ind;
    }

    void PrintVisitor::Visit( const AnnotationEntry & annotation_entry )
    {
        std::cout << "AnnotationEntry" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Type{ " << annotation_entry.m_Type << " }" << endl_ind
            << "Name{ " << annotation_entry.m_Name << " }" << endl_ind
            << "Value{ " << annotation_entry.m_Value << " }" << endl_ind
            << dec_ind << " }" << endl_ind;
    }

    void PrintVisitor::Visit( const TextureDeclaration & declaration )
    {
        std::cout
            << "TextureDeclaration" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Type{ " << declaration.m_Type << " }" << endl_ind
            << "Name{ " << declaration.m_Name << " }" << endl_ind
            << "Semantic{ " << declaration.m_Semantic << " }" << endl_ind;

        if( declaration.m_Annotations )
        {
            declaration.m_Annotations->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const SamplerDeclaration & declaration )
    {
        std::cout
            << "SamplerDeclaration" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Type{ " << declaration.m_Type << " }" << endl_ind
            << "Name{ " << declaration.m_Name << " }" << endl_ind;

        VisitTable( *this, declaration.m_BodyTable );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const SamplerBody & body )
    {
        std::cout
            << "SamplerBody" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << body.m_Name << " }" << endl_ind
            << "Value{ " << body.m_Value << " }" << endl_ind
            << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const StructDefinition & definition )
    {
        std::cout
            << "StructDefinition" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << definition.m_Name << " }" << endl_ind;

        std::vector<StructDefinition::Member>::const_iterator it, end = definition.m_MemberTable.cend();

        for( it = definition.m_MemberTable.cbegin(); it != end; ++it )
        {
            std::cout
                << "Member" << endl_ind
                << "{ " << inc_ind << endl_ind
                << "Name{ " << (*it).m_Name << " }" << endl_ind;

            (*it).m_Type->Visit( *this );

            std::cout
                << "Semantic{ " << (*it).m_Semantic << " }" << endl_ind
                << "InterpolationModifier{" << (*it).m_InterpolationModifier << " }" << endl_ind
                << dec_ind << endl_ind << "}" << endl_ind;
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    // Expressions
    void PrintVisitor::Visit( const LiteralExpression & expression )
    {
        std::cout
            << "LiteralExpression" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Type{ " << expression.m_Type << " }" << endl_ind
            << "Value{ " << expression.m_Value << " }" << endl_ind
            << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const VariableExpression & expression )
    {
        std::cout
            << "VariableExpression" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << expression.m_Name << " }" << endl_ind;

        if( expression.m_SubscriptExpression )
        {
            expression.m_SubscriptExpression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const UnaryOperationExpression & expression )
    {
        std::cout
            << "UnaryOperationExpression" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Operation{ " << expression.m_Operation << " }" << endl_ind;

        if( expression.m_Expression )
        {
            expression.m_Expression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const BinaryOperationExpression & expression )
    {
        std::cout
            << "BinaryOperationExpression" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Operation{ " << expression.m_Operation << " }" << endl_ind;

        if( expression.m_LeftExpression )
        {
            expression.m_LeftExpression->Visit( *this );
        }

        if( expression.m_RightExpression )
        {
            expression.m_RightExpression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const CallExpression & expression )
    {
        std::cout
            << "CallExpression" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << expression.m_Name << " }" << endl_ind;

        if( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const ArgumentExpressionList & list )
    {
        std::cout
            << "ArgumentExpressionList" << endl_ind
            << "{ " << inc_ind << endl_ind;

        VisitTable( *this, list.m_ExpressionList );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const Swizzle & swizzle )
    {
        std::cout
            << "Swizzle" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Swizzle{ " << swizzle.m_Swizzle << " }" << endl_ind
            << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const PostfixSuffixCall & postfix_suffix )
    {
        std::cout
            << "PostfixSuffixCall" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( postfix_suffix.m_CallExpression )
        {
            postfix_suffix.m_CallExpression->Visit( *this );
        }

        if( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const PostfixSuffixVariable & postfix_suffix )
    {
        std::cout
            << "PostfixSuffixVariable" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( postfix_suffix.m_VariableExpression )
        {
            postfix_suffix.m_VariableExpression->Visit( *this );
        }

        if( postfix_suffix.m_Suffix )
        {
            postfix_suffix.m_Suffix->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const ConstructorExpression & expression )
    {
        std::cout
            << "ConstructorExpression" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( expression.m_Type )
        {
            expression.m_Type->Visit( *this );
        }

        if( expression.m_ArgumentExpressionList )
        {
            expression.m_ArgumentExpressionList->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const ConditionalExpression & expression )
    {
        std::cout
            << "ConditionalExpression" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( expression.m_Condition )
        {
            expression.m_Condition->Visit( *this );
        }

        if( expression.m_IfTrue )
        {
            expression.m_IfTrue->Visit( *this );
        }

        if( expression.m_IfFalse )
        {
            expression.m_IfFalse->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const LValueExpression & expression )
    {
        std::cout
            << "LValueExpression" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( expression.m_VariableExpression )
        {
            expression.m_VariableExpression->Visit( *this );
        }

        if( expression.m_Suffix )
        {
            expression.m_Suffix->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const PreModifyExpression & expression )
    {
        std::cout
            << "PreModifyExpression" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Operator{ " << expression.m_Operator << " }" << endl_ind;

        std::cout << " }" << endl_ind;

        if( expression.m_Expression )
        {
            expression.m_Expression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const PostModifyExpression & expression )
    {
        std::cout
            << "PostModifyExpression" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Operator{ " << expression.m_Operator << " }" << endl_ind;

        if( expression.m_Expression )
        {
            expression.m_Expression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const CastExpression & expression )
    {
        std::cout
            << "CastExpression" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( expression.m_Type )
        {
            expression.m_Type->Visit( *this );
        }

        std::cout << "ArraySize{ " << expression.m_ArraySize << " }" << endl_ind;

        if( expression.m_Expression )
        {
            expression.m_Expression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const AssignmentExpression & expression )
    {
        std::cout
            << "AssignmentExpression" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( expression.m_LValueExpression )
        {
            expression.m_LValueExpression->Visit( *this );
        }

        std::cout << "ArraySize{ " << expression.m_Operator << " }" << endl_ind;

        if( expression.m_Expression )
        {
            expression.m_Expression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }


    // Statements

    void PrintVisitor::Visit( const ReturnStatement & statement )
    {
        std::cout
            << "ReturnStatement" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( statement.m_Expression )
        {
            statement.m_Expression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const BreakStatement & /*statement*/ )
    {
        std::cout << "BreakStatement{}" << endl_ind;
    }

    void PrintVisitor::Visit( const ContinueStatement & /*statement*/ )
    {
        std::cout << "ContinueStatement{}" << endl_ind;
    }

    void PrintVisitor::Visit( const DiscardStatement & /*statement*/ )
    {
        std::cout << "DiscardStatement{}" << endl_ind;
    }

    void PrintVisitor::Visit( const EmptyStatement & /*statement*/ )
    {
        std::cout << "EmptyStatement{}" << endl_ind;
    }

    void PrintVisitor::Visit( const ExpressionStatement & statement )
    {
        std::cout
            << "ExpressionStatement" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( statement.m_Expression )
        {
            statement.m_Expression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const IfStatement & statement )
    {
        std::cout
            << "IfStatement" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( statement.m_Condition )
        {
            statement.m_Condition->Visit( *this );
        }

        if( statement.m_ThenStatement )
        {
            statement.m_ThenStatement->Visit( *this );
        }

        if( statement.m_ElseStatement )
        {
            statement.m_ElseStatement->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const WhileStatement & statement )
    {
        std::cout
            << "WhileStatement" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( statement.m_Condition )
        {
            statement.m_Condition->Visit( *this );
        }

        if( statement.m_Statement )
        {
            statement.m_Statement->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const DoWhileStatement & statement )
    {
        std::cout
            << "DoWhileStatement" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( statement.m_Condition )
        {
            statement.m_Condition->Visit( *this );
        }

        if( statement.m_Statement )
        {
            statement.m_Statement->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( const BlockStatement & statement )
    {
        std::cout
            << "BlockStatement" << endl_ind
            << "{ " << inc_ind << endl_ind;

        VisitTable( *this, statement.m_StatementTable );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

};
