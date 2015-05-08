#include <hlsl_parser/hlsl.h>
#include <ast/print_visitor.h>
#include <ast/node.h>
#include <generation/code_generator.h>
#include <generation/technique_generator.h>
#include <tclap/CmdLine.h>
#include <ast/printer/hlsl_printer.h>
#include <base/console_error_handler.h>

int main( int argument_count, const char* argument_table[] )
{
    try
    {
        TCLAP::CmdLine cmd( "ShaderShaker" );

        TCLAP::MultiArg<std::string> semantic_argument("s","semantic","semantic to output",true,"string", cmd);
        TCLAP::MultiArg<std::string> input_semantic_argument("i","input_semantic","semantic available for input",true,"string", cmd);
        TCLAP::MultiArg<std::string> interpolator_semantic_argument(
            "n","interpolator_semantic",
            "semantic used between PS and VS. If no interpolator semantic is given, only one program is generated",
            false,"string", cmd);
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

            definition = Generation::FragmentDefinition::GenerateFragment( *translation_unit );

            definition_table.push_back( definition );
        }

        Base::ErrorHandlerInterface::Ref
            error_handler = new Base::ConsoleErrorHandler;

        if( !interpolator_semantic_argument.isSet() )
        {
            Base::ObjectRef<AST::TranslationUnit>
                generated_code;
            std::vector<std::string>
                used_semantic_set;
            Generation::CodeGenerator
                code_generator;

            code_generator.GenerateShader(
                generated_code,
                used_semantic_set,
                definition_table,
                semantic_argument.getValue(),
                input_semantic_argument.getValue(),
                *error_handler
                );

            AST::HLSLPrinter printer ( std::cout );

            generated_code->Visit( printer );

        }
        else
        {
            Generation::TechniqueGenerator
                generator;
            Base::ObjectRef<AST::TranslationUnit>
                pixel_code,
                vertex_code;
            std::vector<std::string>
                used_input_semantic_table;

            generator.SetOutputSemanticTable( semantic_argument.getValue() );
            generator.SetInputSemanticTable( input_semantic_argument.getValue() );
            generator.SetInterpolatorSemanticTable( interpolator_semantic_argument.getValue() );

            if( !generator.Generate(
                    vertex_code,
                    pixel_code,
                    used_input_semantic_table,
                    definition_table,
                    *error_handler
                    )
                )
            {
                std::cerr << "No code generated, exiting" << std::endl;
                return 1;
            }

            AST::HLSLPrinter printer ( std::cout );

            std::cout << "Vertex Shader : " << std::endl;
            vertex_code->Visit( printer );
            std::cout << "Pixel Shader : " << std::endl;
            pixel_code->Visit( printer );
        }
    }
    catch( TCLAP::ArgException &e )
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

    return 0;
}