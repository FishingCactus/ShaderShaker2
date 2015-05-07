#include "catch.hpp"
#include "ast/node.h"
#include "ast/legalizer/legalizer.h"
#include <sstream>

TEST_CASE( "Legalizer", "" )
{
    SECTION( "Float declaration with integer initial value is converted" )
    {
        AST::VariableDeclaration
            node;

        node.SetType( new AST::Type( "float" ) );
        node.AddBody( new AST::VariableDeclarationBody( "intensity" ) );
        node.m_BodyTable.back()->m_InitialValue = new AST::InitialValue();

        AST::LiteralExpression
            * literal_expression;

        literal_expression = new AST::LiteralExpression( AST::LiteralExpression::Int, "123" );

        node.m_BodyTable.back()->m_InitialValue->AddExpression( literal_expression );

        AST::Legalizer
            legalizer;

        node.Visit( legalizer );

        CHECK( literal_expression->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression->m_Value == "123.0f" );
    }

    SECTION( "Float declaration with missing f is converted" )
    {
        AST::VariableDeclaration
            node;

        node.SetType( new AST::Type( "float" ) );
        node.AddBody( new AST::VariableDeclarationBody( "intensity" ) );
        node.m_BodyTable.back()->m_InitialValue = new AST::InitialValue();

        AST::LiteralExpression
            * literal_expression;

        literal_expression = new AST::LiteralExpression( AST::LiteralExpression::Float, "123.0" );

        node.m_BodyTable.back()->m_InitialValue->AddExpression( literal_expression );

        AST::Legalizer
            legalizer;

        node.Visit( legalizer );

        CHECK( literal_expression->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression->m_Value == "123.0f" );
    }

    SECTION( "Multiple body declarations are converted" )
    {
        AST::VariableDeclaration
            node;

        node.SetType( new AST::Type( "float" ) );
        node.AddBody( new AST::VariableDeclarationBody( "intensity" ) );
        node.m_BodyTable.back()->m_InitialValue = new AST::InitialValue();
        auto literal_expression1 = new AST::LiteralExpression( AST::LiteralExpression::Float, "123.0" );
        node.m_BodyTable.back()->m_InitialValue->AddExpression( literal_expression1 );

        node.AddBody( new AST::VariableDeclarationBody( "length" ) );
        node.m_BodyTable.back()->m_InitialValue = new AST::InitialValue();
        auto literal_expression2 = new AST::LiteralExpression( AST::LiteralExpression::Int, "456" );
        node.m_BodyTable.back()->m_InitialValue->AddExpression( literal_expression2 );

        AST::Legalizer
            legalizer;

        node.Visit( legalizer );

        CHECK( literal_expression1->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression1->m_Value == "123.0f" );

        CHECK( literal_expression2->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression2->m_Value == "456.0f" );
    }

}