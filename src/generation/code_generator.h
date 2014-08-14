#ifndef CODE_GENERATOR_H
    #define CODE_GENERATOR_H

    #include <string>
    #include <vector>
    #include <ast/node.h>
    #include <base/object_ref.h>
    #include "function_definition.h"
    #include "graph.h"

    namespace AST{struct FunctionDeclaration;}
    namespace AST{struct FunctionDeclaration;}

    namespace Generation
    {
        class CodeGenerator
        {

        public:

            Base::ObjectRef<AST::TranslationUnit> GenerateShader(
                const std::vector<std::string> & fragment_name_table,
                const std::vector<std::string> & semantic_table,
                const std::vector<std::string> & input_semantic_set
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

            std::set<std::string>
                m_OutputSemanticSet,
                m_InputSemanticSet,
                m_UsedSemanticSet;
        };
    }


#endif