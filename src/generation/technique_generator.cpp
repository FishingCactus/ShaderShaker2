#include "technique_generator.h"
#include "code_generator.h"

namespace Generation
{
    bool TechniqueGenerator::Generate(
        Base::ObjectRef<AST::TranslationUnit> & vertex_program,
        Base::ObjectRef<AST::TranslationUnit> & pixel_program,
        std::vector<std::string> & input_semantic_table,
        const std::vector<Base::ObjectRef<FragmentDefinition> > & definition_table,
        Base::ErrorHandlerInterface & error_handler
        ) const
    {
        std::vector<std::string>
            pixel_used_semantic_set;
        Generation::CodeGenerator
            code_generator;
        std::vector<std::string>
            interpolator_semantic_list;

        std::copy(
            m_InputSemanticTable.begin(),
            m_InputSemanticTable.end(),
            std::back_inserter( interpolator_semantic_list )
            );

        std::copy(
            m_InterpolatorSemanticTable.begin(),
            m_InterpolatorSemanticTable.end(),
            std::back_inserter( interpolator_semantic_list )
            );

        code_generator.GenerateShader(
            pixel_program,
            pixel_used_semantic_set,
            definition_table,
            m_OutputSemanticTable,
            interpolator_semantic_list,
            error_handler
            );

        if( !pixel_program )
        {
            return false;
        }

        code_generator.GenerateShader(
            vertex_program,
            input_semantic_table,
            definition_table,
            pixel_used_semantic_set,
            m_InputSemanticTable,
            error_handler
            );

        if( !vertex_program )
        {
            pixel_program = 0;
            return false;
        }

        return true;
    }

}