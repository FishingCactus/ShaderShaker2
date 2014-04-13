#include "catch.hpp"
#include "ast/node.h"
#include "parser_helper.h"

TEST_CASE( "Annotations are parsed", "[parser]" )
{
    AST::Annotations * annotations = 0;

    SECTION( "Empty annotation is parsed" )
    {
        const char code[] = " <> ";
        Parser parser( code, sizeof( code ) - 1 );

        annotations = parser.m_Parser.annotations();

        REQUIRE( annotations );
        CHECK( annotations->m_AnnotationTable.size() == 0 );
    }

    SECTION( "Single string annotation is parsed" )
    {
        const char code[] = " <string UIWidget=\"None\";> ";
        Parser parser( code, sizeof( code ) - 1 );

        annotations = parser.m_Parser.annotations();

        REQUIRE( annotations );
        REQUIRE( annotations->m_AnnotationTable.size() == 1 );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Type == "string" );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Name == "UIWidget" );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Value == "\"None\"" );
    }

    SECTION( "Single bool annotation is parsed" )
    {
        const char code[] = " <bool display=true;> ";
        Parser parser( code, sizeof( code ) - 1 );

        annotations = parser.m_Parser.annotations();

        REQUIRE( annotations );
        REQUIRE( annotations->m_AnnotationTable.size() == 1 );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Type == "bool" );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Name == "display" );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Value == "true" );
    }

    SECTION( "Single int annotation is parsed" )
    {
        const char code[] = " <int value=12;> ";
        Parser parser( code, sizeof( code ) - 1 );

        annotations = parser.m_Parser.annotations();

        REQUIRE( annotations );
        REQUIRE( annotations->m_AnnotationTable.size() == 1 );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Type == "int" );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Name == "value" );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Value == "12" );
    }


    SECTION( "Single float annotation is parsed" )
    {
        const char code[] = " <float value=12.0;> ";
        Parser parser( code, sizeof( code ) - 1 );

        annotations = parser.m_Parser.annotations();

        REQUIRE( annotations );
        REQUIRE( annotations->m_AnnotationTable.size() == 1 );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Type == "float" );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Name == "value" );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Value == "12.0" );
    }

    SECTION( "Multiple annotations are parsed" )
    {
        const char code[] = " <float value=12.0; int test=3; bool display = false; string UIWidget=\"None\"; > ";
        Parser parser( code, sizeof( code ) - 1 );

        annotations = parser.m_Parser.annotations();

        REQUIRE( annotations );
        REQUIRE( annotations->m_AnnotationTable.size() == 4 );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Type == "float" );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Name == "value" );
        CHECK( annotations->m_AnnotationTable[ 0 ]->m_Value == "12.0" );
        CHECK( annotations->m_AnnotationTable[ 1 ]->m_Type == "int" );
        CHECK( annotations->m_AnnotationTable[ 1 ]->m_Name == "test" );
        CHECK( annotations->m_AnnotationTable[ 1 ]->m_Value == "3" );
        CHECK( annotations->m_AnnotationTable[ 2 ]->m_Type == "bool" );
        CHECK( annotations->m_AnnotationTable[ 2 ]->m_Name == "display" );
        CHECK( annotations->m_AnnotationTable[ 2 ]->m_Value == "false" );
        CHECK( annotations->m_AnnotationTable[ 3 ]->m_Type == "string" );
        CHECK( annotations->m_AnnotationTable[ 3 ]->m_Name == "UIWidget" );
        CHECK( annotations->m_AnnotationTable[ 3 ]->m_Value == "\"None\"" );
    }

    delete annotations;
}
