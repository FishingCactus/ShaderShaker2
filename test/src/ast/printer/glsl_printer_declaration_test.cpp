#include "catch.hpp"
#include "ast/node.h"
#include "ast/printer/glsl_printer.h"
#include <sstream>

TEST_CASE( "[GLSLPrinter] Variable declarations are printed", "[ast][glsl][printer]" )
{
    SECTION( "Storage class modifiers are printed" )
    {
        AST::StorageClass
            node( "attribute" );
        std::ostringstream
            output;
        AST::GLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "attribute" );
    }

    SECTION( "Type modifiers are printed" )
    {
        AST::TypeModifier
            node( "const" );
        std::ostringstream
            output;
        AST::GLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "const" );
    }

    SECTION( "Storage class modifier followed by type modifier are printed" )
    {
        AST::VariableDeclaration
            node;
        std::ostringstream
            output;
        AST::GLSLPrinter
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
        AST::GLSLPrinter
            printer( output );

        node.SetType( new AST::Type( "float" ) );
        node.AddBody( new AST::VariableDeclarationBody( "intensity" ) );
        node.Visit( printer );

        CHECK( output.str() == "float\n\tintensity;\n" );
    }

    SECTION( "Types are translated" )
    {
        AST::Type
            node( "float4" );
        std::ostringstream
            output;
        AST::GLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "vec4" );
    }

    SECTION( "Variable declaration names are printed" )
    {
        AST::VariableDeclarationBody
            node( "view_space_light_direction" );
        std::ostringstream
            output;
        AST::GLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "view_space_light_direction" );
    }

    SECTION( "Variable declaration semantics are ignored" )
    {
        AST::VariableDeclarationBody
            node( "view_space_light_direction" );
        std::ostringstream
            output;
        AST::GLSLPrinter
            printer( output );

        node.m_Semantic = "LIGHT_DIRECTION";

        node.Visit( printer );

        CHECK( output.str() == "view_space_light_direction" );
    }

    SECTION( "Variable declaration initial scalar values are printed" )
    {
        AST::VariableDeclarationBody
            node( "intensity" );
        std::ostringstream
            output;
        AST::GLSLPrinter
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
        AST::GLSLPrinter
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

    SECTION( "Annotation entries are ignored" )
    {
        AST::AnnotationEntry
            node( "float", "minimum", "0.0f" );
        std::ostringstream
            output;
        AST::GLSLPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "" );
    }

    SECTION( "Annotations are ignored" )
    {
        AST::Annotations
            node;
        std::ostringstream
            output;
        AST::GLSLPrinter
            printer( output );

        node.AddEntry( new AST::AnnotationEntry( "float", "minimum", "0.0f" ) );
        node.AddEntry( new AST::AnnotationEntry( "float", "maximum", "1.0f" ) );

        node.Visit( printer );

        CHECK( output.str() == "" );
    }

    SECTION( "Variable declaration annotations are ignored" )
    {
        AST::VariableDeclarationBody
            node( "intensity" );
        std::ostringstream
            output;
        AST::GLSLPrinter
            printer( output );

        node.m_Annotations = new AST::Annotations();
        node.m_Annotations->AddEntry( new AST::AnnotationEntry( "float", "minimum", "0.0f" ) );
        node.m_Annotations->AddEntry( new AST::AnnotationEntry( "float", "maximum", "1.0f" ) );

        node.Visit( printer );

        CHECK( output.str() == "intensity" );
    }
}
