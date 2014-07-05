#include "print_visitor.h"

#include <assert.h>
#include <iostream>
#include "ast/node.h"
#include "utils/indentation.h"

namespace AST
{

    template< class _Table_ >
    void VisitTable( Visitor & visitor, _Table_ & table )
    {
        typename _Table_::iterator
            it = table.begin(),
            end = table.end();

        for( ; it != end; ++it )
        {
            (*it)->Visit( visitor );
        }
    }

    void PrintVisitor::Visit( Node & node )
    {
        assert(! "Some node type might not be supported in the visitor" );

    }

    void PrintVisitor::Visit( TranslationUnit & translation_unit )
    {
        std::cout
            << "TranslationUnit" << endl_ind
            << "{" << inc_ind << endl_ind;

        VisitTable( *this, translation_unit.m_GlobalDeclarationTable );
        VisitTable( *this, translation_unit.m_TechniqueTable );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( VariableDeclaration & variable_declaration )
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

    void PrintVisitor::Visit( IntrinsicType & type )
    {
        std::cout << "IntrinsicType{ " << type.m_Name << " }" << endl_ind;

    }

    void PrintVisitor::Visit( UserDefinedType & type )
    {
        std::cout << "UserDefinedType{ " << type.m_Name << " }" << endl_ind;

    }

    void PrintVisitor::Visit( SamplerType & type )
    {
        std::cout << "SamplerType{ " << type.m_Name << " }" << endl_ind;
    }

    void PrintVisitor::Visit( TypeModifier & modifier )
    {
        std::cout << "TypeModifier{ " << modifier.m_Value << " }" << endl_ind;

    }

    void PrintVisitor::Visit( StorageClass & storage_class )
    {
        std::cout << "StorageClass{ " << storage_class.m_Value << " }" << endl_ind;
    }

    void PrintVisitor::Visit( VariableDeclarationBody & body )
    {

        std::cout
            << "VariableDeclarationBody" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << body.m_Name << " }" << endl_ind;

        if( !body.m_Semantic.empty() )
        {
            std::cout << "Semantic{ " << body.m_Semantic << " }" << endl_ind;
        }

        if( body.m_InitialValue )
        {
            std::cout << "InitialValue{ " << body.m_InitialValue << " }" << endl_ind;
        }

        if( body.m_ArraySize != 0 )
        {
            std::cout << "ArraySize{ " << body.m_ArraySize << " }" << endl_ind;
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;

    }

    void PrintVisitor::Visit( TextureDeclaration & declaration )
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

    void PrintVisitor::Visit( SamplerDeclaration & declaration )
    {
        std::cout
            << "SamplerDeclaration" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Type{ " << declaration.m_Type << " }" << endl_ind
            << "Name{ " << declaration.m_Name << " }" << endl_ind;

        VisitTable( *this, declaration.m_BodyTable );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( SamplerBody & body )
    {
        std::cout
            << "SamplerBody" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << body.m_Name << " }" << endl_ind
            << "Value{ " << body.m_Value << " }" << endl_ind
            << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( StructDefinition & definition )
    {
        std::cout
            << "StructDefinition" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << definition.m_Name << " }" << endl_ind;

        std::vector<StructDefinition::Member>::iterator it, end = definition.m_MemberTable.end();

        for( it = definition.m_MemberTable.begin(); it != end; ++it )
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
    void PrintVisitor::Visit( LiteralExpression & expression )
    {
        std::cout
            << "LiteralExpression" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Type{ " << expression.m_Type << " }" << endl_ind
            << "Value{ " << expression.m_Value << " }" << endl_ind
            << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( VariableExpression & expression )
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

    void PrintVisitor::Visit( UnaryOperationExpression & expression )
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

    void PrintVisitor::Visit( BinaryOperationExpression & expression )
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

    void PrintVisitor::Visit( CallExpression & expression )
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

    void PrintVisitor::Visit( ArgumentExpressionList & list )
    {
        std::cout
            << "ArgumentExpressionList" << endl_ind
            << "{ " << inc_ind << endl_ind;

        VisitTable( *this, list.m_ExpressionList );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( Swizzle & swizzle )
    {
        std::cout
            << "Swizzle" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Swizzle{ " << swizzle.m_Swizzle << " }" << endl_ind
            << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( PostfixSuffixCall & postfix_suffix )
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

    void PrintVisitor::Visit( PostfixSuffixVariable & postfix_suffix )
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

    void PrintVisitor::Visit( ConstructorExpression & expression )
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

    void PrintVisitor::Visit( ConditionalExpression & expression )
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

    void PrintVisitor::Visit( LValueExpression & expression )
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

    void PrintVisitor::Visit( PreModifyExpression & expression )
    {
        std::cout
            << "PreModifyExpression" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Operator{ ";

        switch( expression.m_Operator )
        {
            case SelfModifyOperator_PlusPlus:
                std::cout << "++";
                break;

            case SelfModifyOperator_MinusMinus:
                std::cout << "--";
                break;

            default:
                assert( !"Unsupported enum" );
        }

        std::cout << " }" << endl_ind;

        if( expression.m_Expression )
        {
            expression.m_Expression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( PostModifyExpression & expression )
    {
        std::cout
            << "PostModifyExpression" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Operator{ ";

        switch( expression.m_Operator )
        {
            case SelfModifyOperator_PlusPlus:
                std::cout << "++";
                break;

            case SelfModifyOperator_MinusMinus:
                std::cout << "--";
                break;

            default:
                assert( !"Unsupported enum" );
        }

        std::cout << " }" << endl_ind;

        if( expression.m_Expression )
        {
            expression.m_Expression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( CastExpression & expression )
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

    void PrintVisitor::Visit( AssignmentExpression & expression )
    {
        std::cout
            << "AssignmentExpression" << endl_ind
            << "{ " << inc_ind << endl_ind;

        if( expression.m_LValueExpression )
        {
            expression.m_LValueExpression->Visit( *this );
        }

        std::cout << "ArraySize{ ";

        switch( expression.m_Operator )
        {
            case AssignmentOperator_Assign : std::cout << "="; break;
            case AssignmentOperator_Multiply : std::cout << "*="; break;
            case AssignmentOperator_Divide : std::cout << "/="; break;
            case AssignmentOperator_Add : std::cout << "+="; break;
            case AssignmentOperator_Subtract : std::cout << "-="; break;
            case AssignmentOperator_BitwiseAnd : std::cout << "&="; break;
            case AssignmentOperator_BitwiseOr : std::cout << "|="; break;
            case AssignmentOperator_BitwiseXor : std::cout << "^="; break;
            case AssignmentOperator_LeftShift : std::cout << "<<="; break;
            case AssignmentOperator_RightShift : std::cout << ">>="; break;
            default : assert( !"Unsupported" );
        }

        std::cout << " }" << endl_ind;

        if( expression.m_Expression )
        {
            expression.m_Expression->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }


    // Statements

    void PrintVisitor::Visit( ReturnStatement & statement )
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

    void PrintVisitor::Visit( BreakStatement & statement )
    {
        std::cout << "BreakStatement{}" << endl_ind;
    }

    void PrintVisitor::Visit( ContinueStatement & statement )
    {
        std::cout << "ContinueStatement{}" << endl_ind;
    }

    void PrintVisitor::Visit( DiscardStatement & statement )
    {
        std::cout << "DiscardStatement{}" << endl_ind;
    }

    void PrintVisitor::Visit( EmptyStatement & statement )
    {
        std::cout << "EmptyStatement{}" << endl_ind;
    }

    void PrintVisitor::Visit( ExpressionStatement & statement )
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

    void PrintVisitor::Visit( IfStatement & statement )
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

    void PrintVisitor::Visit( WhileStatement & statement )
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

    void PrintVisitor::Visit( DoWhileStatement & statement )
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

    void PrintVisitor::Visit( BlockStatement & statement )
    {
        std::cout
            << "BlockStatement" << endl_ind
            << "{ " << inc_ind << endl_ind;

        VisitTable( *this, statement.m_StatementTable );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

};