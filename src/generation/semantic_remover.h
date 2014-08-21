#ifndef SEMANTIC_REMOVER_H
    #define SEMANTIC_REMOVER_H

    #include <ast/empty_visitor.h>

    namespace Generation
    {
        class SemanticRemover : public AST::EmptyVisitor
        {
            virtual void Visit( AST::TranslationUnit & translation_unit ) override;
            virtual void Visit( AST::FunctionDeclaration & function_declaration ) override;
            virtual void Visit( AST::ArgumentList & argument_list ) override;
            virtual void Visit( AST::Argument & argument ) override;
        };
    }

#endif