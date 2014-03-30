#include "hlsl_parser/hlsl.h"
#include "ast/print_visitor.h"
#include "ast/node.h"


int main( int argument_count, const char* argument_table[] )
{
    AST::TranslationUnit * unit = ParseHLSL( argument_table[ 1 ] );

    AST::PrintVisitor print_visitor;

    unit->Visit( print_visitor );

    return 0;
}