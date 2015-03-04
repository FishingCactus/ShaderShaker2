#ifndef CODE_GENERATOR_H
    #define CODE_GENERATOR_H

    #include <string>
    #include <vector>
    #include <ast/node.h>
    #include <base/object_ref.h>
    #include <base/error_handler_interface.h>
    #include "function_definition.h"
    #include "graph.h"

    namespace AST{struct FunctionDeclaration;}
    namespace AST{struct FunctionDeclaration;}

    namespace Generation
    {
        class CodeGenerator
        {

        public:

            void GenerateShader(
                Base::ObjectRef<AST::TranslationUnit> & generated_shader,
                std::vector<std::string> & used_semantic_set,
                const std::vector<Base::ObjectRef<FragmentDefinition> > & definition_table,
                const std::vector<std::string> & semantic_table,
                const std::vector<std::string> & input_semantic_set,
                Base::ErrorHandlerInterface & error_handler
                );

        private:

            bool FindMatchingFunction(
                FunctionDefinition::Ref & function,
                std::set<FunctionDefinition::Ref> & used_function_set,
                const std::set<std::string> & semantic_set,
                const std::vector<FragmentDefinition::Ref > & definition_table
                );

            void RemoveInputSemantics( std::set<std::string> & semantic_set );

            Base::ObjectRef<AST::FunctionDeclaration> GenerateCodeFromGraph(
                const Graph & graph
                );

            Graph::Ref GenerateGraph(
                const std::vector<FragmentDefinition::Ref > & fragment_table
                );

            void MergeTranslationUnit(
                AST::TranslationUnit & destination_translation_unit,
                const std::vector<Base::ObjectRef<AST::TranslationUnit> > & translation_unit_table
                );

            bool ValidatesGraph(
                const Graph & graph
                ) const;

            std::set<std::string>
                m_OutputSemanticSet,
                m_InputSemanticSet,
                m_UsedSemanticSet;
            std::vector<Base::ObjectRef<AST::TranslationUnit> >
                m_UsedTranslationUnitSet;
            mutable Base::ErrorHandlerInterface::Ref
                m_ErrorHandler;

        };
    }


#endif