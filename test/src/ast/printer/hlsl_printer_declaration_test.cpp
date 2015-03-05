#include "catch.hpp"
#include "ast/node.h"
#include "ast/printer/hlsl_printer.h"
#include <sstream>

TEST_CASE( "Variable declarations are printed", "[ast][hlsl][printer]" )
{
    SECTION( "Storage class modifiers are printed" )
    {
        AST::StorageClass
            node( "static" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "static" );
    }

    SECTION( "Multiple storage class modifiers are printed" )
    {
        AST::VariableDeclaration
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.AddStorageClass( new AST::StorageClass( "extern" ) );
        node.AddStorageClass( new AST::StorageClass( "uniform" ) );
        node.SetType( new AST::Type( "float" ) );

        node.Visit( printer );

        CHECK( output.str() == "extern uniform float\n\t;\n" );
    }

    SECTION( "Type modifiers are printed" )
    {
        AST::TypeModifier
            node( "const" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "const" );
    }

    SECTION( "Multiple type modifiers are printed" )
    {
        AST::VariableDeclaration
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.AddTypeModifier( new AST::TypeModifier( "const" ) );
        node.AddTypeModifier( new AST::TypeModifier( "precise" ) );
        node.SetType( new AST::Type( "float" ) );

        node.Visit( printer );

        CHECK( output.str() == "const precise float\n\t;\n" );
    }

    SECTION( "Storage class modifier followed by type modifier are printed" )
    {
        AST::VariableDeclaration
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.AddStorageClass( new AST::StorageClass( "uniform" ) );
        node.AddTypeModifier( new AST::TypeModifier( "const" ) );
        node.SetType( new AST::Type( "float" ) );

        node.Visit( printer );

        CHECK( output.str() == "uniform const float\n\t;\n" );
    }

    SECTION( "Types are printed" )
    {
        AST::VariableDeclaration
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.SetType( new AST::Type( "float4" ) );
        node.AddBody( new AST::VariableDeclarationBody( "view_space_light_direction" ) );
        node.Visit( printer );

        CHECK( output.str() == "float4\n\tview_space_light_direction;\n" );
    }

    SECTION( "Variable declaration names are printed" )
    {
        AST::VariableDeclarationBody
            node( "view_space_light_direction" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "view_space_light_direction" );
    }

    SECTION( "Variable declaration semantics are printed" )
    {
        AST::VariableDeclarationBody
            node( "view_space_light_direction" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.m_Semantic = "LIGHT_DIRECTION";

        node.Visit( printer );

        CHECK( output.str() == "view_space_light_direction : LIGHT_DIRECTION" );
    }

    SECTION( "Variable declaration initial scalar values are printed" )
    {
        AST::VariableDeclarationBody
            node( "intensity" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.m_InitialValue = new AST::InitialValue();
        node.m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "123.4f" ) );

        node.Visit( printer );

        CHECK( output.str() == "intensity = 123.4f" );
    }

    SECTION( "Variable declaration initial vector values are printed" )
    {
        AST::VariableDeclarationBody
            node( "view_space_light_direction" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.m_InitialValue = new AST::InitialValue();
        node.m_InitialValue->m_Vector = true;
        node.m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "0.0f" ) );
        node.m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "0.0f" ) );
        node.m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "1.0f" ) );
        node.m_InitialValue->AddExpression( new AST::LiteralExpression( AST::LiteralExpression::Float, "0.0f" ) );

        node.Visit( printer );

        CHECK( output.str() == "view_space_light_direction = { 0.0f, 0.0f, 1.0f, 0.0f }" );
    }

    SECTION( "Annotation entries are printed" )
    {
        AST::AnnotationEntry
            node( "float", "minimum", "0.0f" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "float minimum = 0.0f; " );
    }

    SECTION( "Annotations are printed" )
    {
        AST::Annotations
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.AddEntry( new AST::AnnotationEntry( "float", "minimum", "0.0f" ) );
        node.AddEntry( new AST::AnnotationEntry( "float", "maximum", "1.0f" ) );

        node.Visit( printer );

        CHECK( output.str() == " < float minimum = 0.0f; float maximum = 1.0f; >" );
    }

    SECTION( "Variable declaration annotations are printed" )
    {
        AST::VariableDeclarationBody
            node( "intensity" );
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.m_Annotations = new AST::Annotations();
        node.m_Annotations->AddEntry( new AST::AnnotationEntry( "float", "minimum", "0.0f" ) );
        node.m_Annotations->AddEntry( new AST::AnnotationEntry( "float", "maximum", "1.0f" ) );

        node.Visit( printer );

        CHECK( output.str() == "intensity < float minimum = 0.0f; float maximum = 1.0f; >" );
    }

    SECTION( "Complete variable declarations are printed" )
    {
        AST::VariableDeclaration
            node;
        std::ostringstream
            output;
        AST::HLSLPrinter
            printer( output );

        node.AddStorageClass( new AST::StorageClass( "uniform" ) );
        node.AddTypeModifier( new AST::TypeModifier( "const" ) );
        node.SetType( new AST::Type( "float" ) );

        node.AddBody( new AST::VariableDeclarationBody( "intensity" ) );
        node.m_BodyTable.back()->m_Semantic = "INTENSITY";
        node.m_BodyTable.back()->m_ArraySize = 4;
        node.m_BodyTable.back()->m_Annotations = new AST::Annotations();
        node.m_BodyTable.back()->m_Annotations->AddEntry( new AST::AnnotationEntry( "float", "step", "0.01f" ) );

        node.AddBody( new AST::VariableDeclarationBody( "fresnel" ) );
        node.m_BodyTable.back()->m_Semantic = "FRESNEL";

        node.Visit( printer );

        CHECK( output.str() == "uniform const float\n\tintensity[4] : INTENSITY < float step = 0.01f; >,\n\tfresnel : FRESNEL;\n" );
    }
}
