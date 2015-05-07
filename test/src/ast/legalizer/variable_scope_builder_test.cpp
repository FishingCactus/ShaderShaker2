#include "catch.hpp"
#include "ast/node.h"
#include "ast/legalizer/variable_scope_builder.h"
#include "../../parser/parser_helper.h"
#include <sstream>

static AST::TranslationUnit * ParseCode(
    const std::string & code
    )
{
    Parser parser( code.c_str(), code.length() );

    return parser.m_Parser.translation_unit();
}

TEST_CASE( "GlobalScope", "Variables are correctly added to the global scope" )
{
    SECTION( "One variable" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f = 123.0f;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float" );
    }

    SECTION( "Multiple variables initialized in same expression" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f1 = 123.0f, f2;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Name == "f1" );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float" );

        CHECK( scope_data.m_GlobalScope.m_Variables[ 1 ]->m_Name == "f2" );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 1 ]->m_Type == "float" );
    }

    SECTION( "Multiple variables initialized in multiple expressions" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f1 = 123.0f; float f2;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Name == "f1" );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float" );

        CHECK( scope_data.m_GlobalScope.m_Variables[ 1 ]->m_Name == "f2" );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 1 ]->m_Type == "float" );
    }

    SECTION( "Multiple variables of different types" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f = 123.0f; int i = 1;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float" );

        CHECK( scope_data.m_GlobalScope.m_Variables[ 1 ]->m_Name == "i" );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 1 ]->m_Type == "int" );
    }
}

TEST_CASE( "FunctionScope", "Functions create a new child scope" )
{
    SECTION( "Void function no argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "void foo() {}" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.empty() );
    }

    SECTION( "Void function one argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "void foo( float f ) {}" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float" );
    }

    SECTION( "Void function multiple arguments" )
    {
        AST::TranslationUnit * unit = ParseCode( "void foo( float f, int i ) {}" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Name == "i" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Type == "int" );
    }

    SECTION( "Result function no argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo() { return 1.0f; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "float" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.empty() );
    }

    SECTION( "Result function one argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo( float f ) { return f; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "float" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float" );
    }

    SECTION( "Result function multiple arguments" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo( float f, int i ) { return f * i; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "float" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Name == "i" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Type == "int" );
    }

    SECTION( "Multiple functions" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo( int i ) { return 10.0f; } int bar( float f ) { return 0; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 2 );

        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "float" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "i" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "int" );

        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Name == "bar" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Type == "int" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ]->m_Type == "float" );
    }

    SECTION( "Variable and function" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f = 1.0f; float foo( int i ) { return 10.0f; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float" );

        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "float" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "i" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "int" );
    }
}

TEST_CASE( "StructureScope", "Structure declarations create a new child scope" )
{
    SECTION( "Structure with one member" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; };" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "VS_OUTPUT" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "Position" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float4" );
    }

    SECTION( "Structure with multiple members" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; float2 TexturePosition; };" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "VS_OUTPUT" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "Position" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float4" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Name == "TexturePosition" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Type == "float2" );
    }

    SECTION( "Multiple structures with multiple members" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; float2 TexturePosition; }; struct PS_OUTPUT { float4 Position; float4 Color; };" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "VS_OUTPUT" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "Position" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float4" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Name == "TexturePosition" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Type == "float2" );

        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Name == "PS_OUTPUT" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Type == "void" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ]->m_Name == "Position" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ]->m_Type == "float4" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 1 ]->m_Name == "Color" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 1 ]->m_Type == "float4" );
    }

    SECTION( "Structures with global variables" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; }; float4x4 WvpXf;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Name == "WvpXf" );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float4x4" );

        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "VS_OUTPUT" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "Position" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float4" );
    }

    SECTION( "Multiple structures with functions" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; }; float foo() { return 1.0f; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "VS_OUTPUT" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "Position" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float4" );

        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Name == "foo" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Type == "float" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables.empty() );
    }

    SECTION( "Multiple structures with global variables and functions" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; }; float foo( int i ) { return 1.0f; } float4x4 WvpXf;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Name == "WvpXf" );
        CHECK( scope_data.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float4x4" );

        CHECK( scope_data.m_GlobalScope.m_Children.size() == 2 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Name == "VS_OUTPUT" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "Position" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float4" );

        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Parent == &scope_data.m_GlobalScope );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Children.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Name == "foo" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Type == "float" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables.size() == 1 );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ]->m_Name == "i" );
        CHECK( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ]->m_Type == "int" );
    }
}