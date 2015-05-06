#include "catch.hpp"
#include "ast/node.h"
#include "ast/legalizer/legalizer.h"
#include <sstream>

TEST_CASE( "Legalizer", "" )
{
    SECTION( "Integers assigned to float are converted" )
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

        /*node.m_InitialValue = new AST::InitialValue();
        node.m_InitialValue->m_Vector = true;
        node.m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "0.0f" ) );
        node.m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "0.0f" ) );
        node.m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "1.0f" ) );
        node.m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "0.0f" ) );*/

        AST::Legalizer
            legalizer;

        node.Visit( legalizer );

        CHECK( literal_expression->m_Type == AST::LiteralExpression::Float );
        CHECK( literal_expression->m_Value == "123.0f" );
    }
}