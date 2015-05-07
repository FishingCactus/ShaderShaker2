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
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float" );
    }

    SECTION( "Multiple variables initialized in same expression" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f1 = 123.0f, f2;" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.size() == 2 );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 0 ]->m_Name == "f1" );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float" );

        CHECK( scope_builder.m_GlobalScope.m_Variables[ 1 ]->m_Name == "f2" );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 1 ]->m_Type == "float" );
    }

    SECTION( "Multiple variables initialized in multiple expressions" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f1 = 123.0f; float f2;" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.size() == 2 );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 0 ]->m_Name == "f1" );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float" );

        CHECK( scope_builder.m_GlobalScope.m_Variables[ 1 ]->m_Name == "f2" );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 1 ]->m_Type == "float" );
    }

    SECTION( "Multiple variables of different types" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f = 123.0f; int i = 1;" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.size() == 2 );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float" );

        CHECK( scope_builder.m_GlobalScope.m_Variables[ 1 ]->m_Name == "i" );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 1 ]->m_Type == "int" );
    }
}

TEST_CASE( "FunctionScope", "Functions create a new child scope" )
{
    SECTION( "Void function no argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "void foo() {}" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_builder.m_GlobalScope );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables.empty() );
    }

    SECTION( "Void function one argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "void foo( float f ) {}" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_builder.m_GlobalScope );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float" );
    }

    SECTION( "Void function multiple arguments" )
    {
        AST::TranslationUnit * unit = ParseCode( "void foo( float f, int i ) {}" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_builder.m_GlobalScope );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Type == "void" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 2 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Name == "i" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Type == "int" );
    }

    SECTION( "Result function no argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo() { return 1.0f; }" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_builder.m_GlobalScope );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Type == "float" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables.empty() );
    }

    SECTION( "Result function one argument" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo( float f ) { return f; }" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_builder.m_GlobalScope );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Type == "float" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float" );
    }

    SECTION( "Result function multiple arguments" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo( float f, int i ) { return f * i; }" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_builder.m_GlobalScope );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Type == "float" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 2 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "float" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Name == "i" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 1 ]->m_Type == "int" );
    }

    SECTION( "Multiple functions" )
    {
        AST::TranslationUnit * unit = ParseCode( "float foo( int i ) { return 10.0f; } int bar( float f ) { return 0; }" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children.size() == 2 );

        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_builder.m_GlobalScope );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Type == "float" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "i" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "int" );

        CHECK( scope_builder.m_GlobalScope.m_Children[ 1 ]->m_Parent == &scope_builder.m_GlobalScope );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 1 ]->m_Children.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 1 ]->m_Name == "bar" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 1 ]->m_Type == "int" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 1 ]->m_Variables.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 1 ]->m_Variables[ 0 ]->m_Type == "float" );
    }

    SECTION( "Variable and function" )
    {
        AST::TranslationUnit * unit = ParseCode( "float f = 1.0f; float foo( int i ) { return 10.0f; }" );
        AST::VariableScopeBuilder scope_builder;
        unit->Visit( scope_builder );

        CHECK( scope_builder.m_GlobalScope.m_Variables.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 0 ]->m_Name == "f" );
        CHECK( scope_builder.m_GlobalScope.m_Variables[ 0 ]->m_Type == "float" );

        CHECK( scope_builder.m_GlobalScope.m_Children.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Parent == &scope_builder.m_GlobalScope );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Children.empty() );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Name == "foo" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Type == "float" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables.size() == 1 );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Name == "i" );
        CHECK( scope_builder.m_GlobalScope.m_Children[ 0 ]->m_Variables[ 0 ]->m_Type == "int" );
    }
}
