#include "catch.hpp"
#include "ast/node.h"
#include "ast/legalizer/legalizer.h"
#include "../../parser/parser_helper.h"
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

    SECTION( "Float declaration with missing decimal is converted" )
    {
        AST::VariableDeclaration
            node;

        node.SetType( new AST::Type( "float" ) );
        node.AddBody( new AST::VariableDeclarationBody( "intensity" ) );
        node.m_BodyTable.back()->m_InitialValue = new AST::InitialValue();

        AST::LiteralExpression
            * literal_expression;

        literal_expression = new AST::LiteralExpression( AST::LiteralExpression::Float, "123.f" );

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

    SECTION( "Vector components are converted" )
    {
        AST::VariableDeclaration
            node;

        node.SetType( new AST::Type( "float4" ) );
        node.AddBody( new AST::VariableDeclarationBody( "view_space_light_direction" ) );
        node.m_BodyTable.back()->m_InitialValue = new AST::InitialValue();
        node.m_BodyTable.back()->m_InitialValue->m_Vector = true;
        node.m_BodyTable.back()->m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "0.0f" ) );
        auto literal_expression1 = new AST::LiteralExpression( AST::LiteralExpression::Int, "0" );
        node.m_BodyTable.back()->m_InitialValue->AddExpression( literal_expression1 );
        node.m_BodyTable.back()->m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "1.0f" ) );
        auto literal_expression2 = new AST::LiteralExpression( AST::LiteralExpression::Float, "1" );
        node.m_BodyTable.back()->m_InitialValue->AddExpression( literal_expression2 );

        AST::Legalizer
            legalizer;

        node.Visit( legalizer );

        CHECK( literal_expression1->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression1->m_Value == "0.0f" );

        CHECK( literal_expression2->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression2->m_Value == "1.0f" );
    }
}