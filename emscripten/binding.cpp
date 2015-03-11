#include <emscripten/bind.h>
#include <hlsl_parser/hlsl.h>
#include <ast/node.h>
#include <ast/printer/hlsl_printer.h>
#include <generation/function_definition.h>
#include <generation/fragment_definition.h>
#include <generation/code_generator.h>
#include <base/text_error_handler.h>
#include <iostream>

namespace emscripten
{
    template<typename PointeeType>
    struct smart_ptr_trait<Base::ObjectRef<PointeeType>>
    {
        typedef Base::ObjectRef<PointeeType> PointerType;
        typedef PointeeType element_type;

        static element_type* get(const PointerType& ptr)
        {
            return const_cast<PointeeType*>( &*ptr );
        }

        static sharing_policy get_sharing_policy()
        {
            return sharing_policy::INTRUSIVE;
        }

        static Base::ObjectRef<PointeeType>* share(PointeeType* p, internal::EM_VAL v)
        {
            assert( "not implemented" );
            return 0;
        }

        static PointerType* construct_null()
        {
            return new PointerType;
        }
    };
}

namespace
{
    emscripten::val LOCAL_GenerateShader(
        Generation::CodeGenerator & code_generator,
        const emscripten::val & js_definition_table,
        const emscripten::val & js_semantic_table,
        const emscripten::val & js_input_semantic_set
        )
    {
        emscripten::val
            result( emscripten::val::object() );
        Base::ObjectRef<AST::TranslationUnit>
            generated_shader;
        std::vector<std::string>
            used_semantic_set;
        Base::TextErrorHandler
            error_handler;
        std::vector<Base::ObjectRef<Generation::FragmentDefinition> >
            definition_table;
        std::vector<std::string>
            semantic_table,
            input_semantic_set;

        definition_table = emscripten::vecFromJSArray<Base::ObjectRef<Generation::FragmentDefinition>>( js_definition_table );
        semantic_table = emscripten::vecFromJSArray<std::string>( js_semantic_table );
        input_semantic_set = emscripten::vecFromJSArray<std::string>( js_input_semantic_set );

        code_generator.GenerateShader(
            generated_shader,
            used_semantic_set,
            definition_table,
            semantic_table,
            input_semantic_set,
            error_handler
            );

        emscripten::val
            js_used_semantic_set( emscripten::val::array() );
        int
            index = 0;

        std::for_each(
            used_semantic_set.begin(), used_semantic_set.end(),
            [&]( const std::string & value ){ js_used_semantic_set.set( index, value );++index; }
            );

        result.set( "Shader", generated_shader );
        result.set( "UsedSemanticSet", js_used_semantic_set );

        const std::string & error_message = error_handler.GetErrorMessage();

        if( !error_message.empty() )
        {
            result.set( "ErrorMessage", error_message );
        }

        return result;
    }

    std::string LOCAL_GetHlslCode(
        AST::TranslationUnit & translation_unit
        )
    {
        std::stringstream
            result;
        AST::HLSLPrinter
            printer( result );

        translation_unit.Visit( printer );

        return result.str();
    }
}

EMSCRIPTEN_BINDINGS(ShaderShaker2)
{
    emscripten::function("ParseHLSL", &HLSL::ParseHLSL );
    emscripten::function( "GenerateFragmentDefinition", &Generation::FragmentDefinition::GenerateFragment );
    emscripten::function( "GetHlslCode", &LOCAL_GetHlslCode );

    emscripten::class_<AST::TranslationUnit>("TranslationUnit")
        .smart_ptr<Base::ObjectRef<AST::TranslationUnit>>("TranslationUnitRef");

    emscripten::class_<Generation::FragmentDefinition>("FragmentDefinition")
        .smart_ptr<Base::ObjectRef<Generation::FragmentDefinition>>( "FragmentDefinitionRef" );

    emscripten::class_<Generation::CodeGenerator>( "CodeGenerator" )
        .constructor()
        .function("GenerateShader", &LOCAL_GenerateShader);
}
