#include "catch.hpp"
#include "ast/node.h"
#include "ast/printer/annotation_printer.h"
#include "ast/annotation_node.h"
#include <sstream>

TEST_CASE( "Annotations", "[ast][annotations][printer]" )
{
    SECTION( "Empty annotations prints empty object" )
    {
        AST::Annotations
            node;
        std::ostringstream
            output;
        AST::AnnotationPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "{}" );
    }

    SECTION( "Annotations with one entry prints 1 element" )
    {
        AST::Annotations
            node;
        std::ostringstream
            output;
        AST::AnnotationPrinter
            printer( output );

        node.AddEntry( new AST::AnnotationEntry( { "float", "Object", "10.0f" } ) );
        node.Visit( printer );

        CHECK( output.str() == R"({"Object":{"type":"float","value":"10.0f"}})");
    }

    SECTION( "Annotations with multiple entries prints comma separated elements" )
    {
        AST::Annotations
            node;
        std::ostringstream
            output;
        AST::AnnotationPrinter
            printer( output );

        node.AddEntry( new AST::AnnotationEntry( { "float", "Object", "10.0f" } ) );
        node.AddEntry( new AST::AnnotationEntry( { "string", "foo", "Hello" } ) );
        node.Visit( printer );

        CHECK( output.str() == R"({"Object":{"type":"float","value":"10.0f"},"foo":{"type":"string","value":"Hello"}})");
    }
}

TEST_CASE( "AnnotationEntry", "[ast][annotationentry][printer]" )
{
    SECTION( "Empty AnnotationEntry prints nothing" )
    {
        AST::AnnotationEntry
            node;
        std::ostringstream
            output;
        AST::AnnotationPrinter
            printer( output );

        node.Visit( printer );

        CHECK( output.str() == "" );
    }

    SECTION( "AnnotationEntry with no type prints nothing" )
    {
        AST::AnnotationEntry
            node;
        std::ostringstream
            output;
        AST::AnnotationPrinter
            printer( output );

        node.m_Name = "Object";
        node.m_Value = "32.0f";

        node.Visit( printer );

        CHECK( output.str() == "" );
    }

    SECTION( "AnnotationEntry with no name prints nothing" )
    {
        AST::AnnotationEntry
            node;
        std::ostringstream
            output;
        AST::AnnotationPrinter
            printer( output );

        node.m_Type = "float";
        node.m_Value = "32.0f";

        node.Visit( printer );

        CHECK( output.str() == "" );
    }

    SECTION( "AnnotationEntry with no value prints nothing" )
    {
        AST::AnnotationEntry
            node;
        std::ostringstream
            output;
        AST::AnnotationPrinter
            printer( output );

        node.m_Type = "float";
        node.m_Name = "Object";

        node.Visit( printer );

        CHECK( output.str() == "" );
    }

    SECTION( "AnnotationEntry with all values prints JSON" )
    {
        AST::AnnotationEntry
            node;
        std::ostringstream
            output;
        AST::AnnotationPrinter
            printer( output );

        node.m_Type = "float";
        node.m_Name = "Object";
        node.m_Value = "10.0f";

        node.Visit( printer );

        CHECK( output.str() == R"("Object":{"type":"float","value":"10.0f"})" );
    }
}
