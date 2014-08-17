#include <hlsl_parser/hlsl.h>
#include <ast/print_visitor.h>
#include <ast/node.h>
#include <generation/code_generator.h>
#include <tclap/CmdLine.h>
#include <ast/printer/hlsl_printer.h>

int main( int argument_count, const char* argument_table[] )
{
    try
    {
        TCLAP::CmdLine cmd( "ShaderShaker" );

        TCLAP::MultiArg<std::string> semantic_argument("s","semantic","semantic to output",true,"string", cmd);
        TCLAP::MultiArg<std::string> input_semantic_argument("i","input_semantic","semantic available for input",true,"string", cmd);
        TCLAP::UnlabeledMultiArg<std::string> fragment_arguments( "fragment", "fragment file path", true, "filepath", cmd);

        cmd.parse( argument_count, argument_table );

        std::vector<Base::ObjectRef<Generation::FragmentDefinition> > definition_table;
        std::vector<std::string>::const_iterator it, end;
        it = fragment_arguments.getValue().begin();
        end = fragment_arguments.getValue().end();

        for(; it!=end; ++it )
        {
            Base::ObjectRef<AST::TranslationUnit>
                translation_unit;
            Base::ObjectRef<Generation::FragmentDefinition>
                definition;

            translation_unit = HLSL::ParseHLSL( (*it).c_str() );

            Generation::FragmentDefinition::GenerateFragment( definition, *translation_unit );

            definition_table.push_back( definition );
        }

        Base::ObjectRef<AST::TranslationUnit> generated_code;

        Generation::CodeGenerator code_generator;

        generated_code = code_generator.GenerateShader(
            definition_table,
            semantic_argument.getValue(),
            input_semantic_argument.getValue()
            );

        AST::HLSLPrinter printer ( std::cout );

        generated_code->Visit( printer );
    }
    catch( TCLAP::ArgException &e )
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

    return 0;
}