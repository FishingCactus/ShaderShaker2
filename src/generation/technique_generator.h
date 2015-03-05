#pragma once

#include <string>
#include <vector>
#include <ast/node.h>
#include <base/object_ref.h>
#include <base/error_handler_interface.h>
#include "function_definition.h"
#include "graph.h"

namespace AST{struct FunctionDeclaration;}

namespace Generation
{
    struct CodeGeneratorHelper;

    class TechniqueGenerator
    {
    public:

        void SetInputSemanticTable(
            const std::vector<std::string> & input_semantic_table
            )
        {
            m_InputSemanticTable = input_semantic_table;
        }

        void SetOutputSemanticTable(
            const std::vector<std::string> & output_semantic_table
            )
        {
            m_OutputSemanticTable = output_semantic_table;
        }

        void SetInterpolatorSemanticTable(
            const std::vector<std::string> & interpolator_semantic_table
            )
        {
            m_InterpolatorSemanticTable = interpolator_semantic_table;
        }

        bool Generate(
            Base::ObjectRef<AST::TranslationUnit> & vertex_program,
            Base::ObjectRef<AST::TranslationUnit> & pixel_program,
            std::vector<std::string> & input_semantic_table,
            const std::vector<Base::ObjectRef<FragmentDefinition> > & definition_table,
            Base::ErrorHandlerInterface & error_handler
            ) const;

    private:

        std::vector<std::string>
            m_OutputSemanticTable,
            m_InterpolatorSemanticTable,
            m_InputSemanticTable;

    };

}