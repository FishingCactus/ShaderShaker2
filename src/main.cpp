#include "hlsl_parser/hlsl.h"


int main( int argument_count, const char* argument_table[] )
{
    AST::TranslationUnit * unit = ParseHLSL( argument_table[ 1 ] );
    
    
	return 0;
}