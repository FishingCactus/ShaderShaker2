#include "hlsl_parser/hlsl.h"
#include "ast/print_visitor.h"
#include "ast/node.h"
#include "generation/code_generator.h"
#include <tclap/CmdLine.h>


int main( int argument_count, const char* argument_table[] )
{
    try
    {
        TCLAP::CmdLine cmd( "ShaderShaker" );

        TCLAP::MultiArg<std::string> semantic_argument("s","semantic","semantic to output",true,"string", cmd);
        TCLAP::MultiArg<std::string> input_semantic_argument("i","input_semantic","semantic available for input",true,"string", cmd);
        TCLAP::UnlabeledMultiArg<std::string> fragment_arguments( "fragment", "fragment file path", true, "filepath", cmd);

        cmd.parse( argument_count, argument_table );

        Base::ObjectRef<AST::TranslationUnit> generated_code;

        Generation::CodeGenerator code_generator;

        generated_code = code_generator.GenerateShader(
            fragment_arguments.getValue(),
            semantic_argument.getValue(),
            input_semantic_argument.getValue()
            );

    }
    catch( TCLAP::ArgException &e )
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

    return 0;
}