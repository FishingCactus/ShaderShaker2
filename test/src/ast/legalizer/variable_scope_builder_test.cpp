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

void CheckVariable(
    const Base::ObjectRef< AST::ScopeBuilder::Variable > & variable,
    const std::string & name,
    const std::string & type
    )
{
    CHECK( variable->m_Name == name );
    CHECK( variable->m_Type == type );
}

void CheckScope(
    const Base::ObjectRef< AST::ScopeBuilder::Scope > & scope,
    const AST::ScopeBuilder::Scope & parent_scope,
    const unsigned int children_scope_count,
    const std::string & scope_name,
    const std::string & scope_type,
    const unsigned int variable_count
    )
{
    CHECK( scope->m_Parent == &parent_scope );
    CHECK( scope->m_Children.size() == children_scope_count );
    CHECK( scope->m_Name == scope_name );
    CHECK( scope->m_Type == scope_type );
    CHECK( scope->m_Variables.size() == variable_count );
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
        CheckVariable( scope_data.m_GlobalScope.m_Variables[ 0 ], "f", "float" );
    }

    SECTION( "Multiple variables initialized in same expression" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f1 = 123.0f, f2;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 2 );
        CheckVariable( scope_data.m_GlobalScope.m_Variables[ 0 ], "f1", "float" );
        CheckVariable( scope_data.m_GlobalScope.m_Variables[ 1 ], "f2", "float" );
    }

    SECTION( "Multiple variables initialized in multiple expressions" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f1 = 123.0f; float f2;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 2 );
        CheckVariable( scope_data.m_GlobalScope.m_Variables[ 0 ], "f1", "float" );
        CheckVariable( scope_data.m_GlobalScope.m_Variables[ 1 ], "f2", "float" );
    }

    SECTION( "Multiple variables of different types" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f = 123.0f; int i = 1;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 2 );
        CheckVariable( scope_data.m_GlobalScope.m_Variables[ 0 ], "f", "float" );
        CheckVariable( scope_data.m_GlobalScope.m_Variables[ 1 ], "i", "int" );
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
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "foo", "void", 0 );
    }

    SECTION( "Void function one argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "void foo( float f ) {}" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "foo", "void", 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "f", "float" );
    }

    SECTION( "Void function multiple arguments" )
    {
        AST::TranslationUnit * unit = ParseCode( "void foo( float f, int i ) {}" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "foo", "void", 2 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "f", "float" );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ], "i", "int" );
    }

    SECTION( "Function with one statement" )
    {
        AST::TranslationUnit * unit = ParseCode( "void foo( float f ) { float g = 1.0f; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "foo", "void", 2 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "f", "float" );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ], "g", "float" );
    }

    SECTION( "Function with multiple statements" )
    {
        AST::TranslationUnit * unit = ParseCode( "void foo( float f ) { float g = 1.0f; int h = 3; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "foo", "void", 3 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "f", "float" );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ], "g", "float" );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 2 ], "h", "int" );
    }

    SECTION( "Result function no argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo() { return 1.0f; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "foo", "float", 0 );
    }

    SECTION( "Result function one argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo( float f ) { return f; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "foo", "float", 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "f", "float" );
    }

    SECTION( "Result function multiple arguments" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo( float f, int i ) { return f * i; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "foo", "float", 2 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "f", "float" );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ], "i", "int" );
    }

    SECTION( "Multiple functions" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo( int i ) { return 10.0f; } int bar( float f ) { return 0; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 2 );

        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "foo", "float", 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "i", "int" );

        CheckScope( scope_data.m_GlobalScope.m_Children[ 1 ], scope_data.m_GlobalScope, 0, "bar", "int", 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ], "f", "float" );
    }

    SECTION( "Variable and function" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f = 1.0f; float foo( int i ) { return 10.0f; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Variables[ 0 ], "f", "float" );

        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "foo", "float", 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "i", "int" );
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
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "VS_OUTPUT", "", 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "Position", "float4" );
    }

    SECTION( "Structure with multiple members" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; float2 TexturePosition; };" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "VS_OUTPUT", "", 2 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "Position", "float4" );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ], "TexturePosition", "float2" );
    }

    SECTION( "Multiple structures with multiple members" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; float2 TexturePosition; }; struct PS_OUTPUT { float4 Position; float4 Color; };" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 2 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "VS_OUTPUT", "", 2 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "Position", "float4" );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ], "TexturePosition", "float2" );

        CheckScope( scope_data.m_GlobalScope.m_Children[ 1 ], scope_data.m_GlobalScope, 0, "PS_OUTPUT", "", 2 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ], "Position", "float4" );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 1 ], "Color", "float4" );
    }

    SECTION( "Structures with global variables" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; }; float4x4 WvpXf;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Variables[ 0 ], "WvpXf", "float4x4" );

        CHECK( scope_data.m_GlobalScope.m_Children.size() == 1 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "VS_OUTPUT", "", 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "Position", "float4" );
    }

    SECTION( "Multiple structures with functions" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; }; float foo() { return 1.0f; }" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_data.m_GlobalScope.m_Children.size() == 2 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "VS_OUTPUT", "", 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "Position", "float4" );

        CheckScope( scope_data.m_GlobalScope.m_Children[ 1 ], scope_data.m_GlobalScope, 0, "foo", "float", 0 );
    }

    SECTION( "Multiple structures with global variables and functions" )
    {
        AST::TranslationUnit * unit = ParseCode( "struct VS_OUTPUT { float4 Position; }; float foo( int i ) { return 1.0f; } float4x4 WvpXf;" );
        AST::ScopeBuilder::ScopeData scope_data;
        AST::VariableScopeBuilder scope_builder( scope_data );
        unit->Visit( scope_builder );

        CHECK( scope_data.m_GlobalScope.m_Variables.size() == 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Variables[ 0 ], "WvpXf", "float4x4" );

        CHECK( scope_data.m_GlobalScope.m_Children.size() == 2 );
        CheckScope( scope_data.m_GlobalScope.m_Children[ 0 ], scope_data.m_GlobalScope, 0, "VS_OUTPUT", "", 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ], "Position", "float4" );

        CheckScope( scope_data.m_GlobalScope.m_Children[ 1 ], scope_data.m_GlobalScope, 0, "foo", "float", 1 );
        CheckVariable( scope_data.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ], "i", "int" );
    }
}