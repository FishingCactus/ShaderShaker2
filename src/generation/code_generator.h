#ifndef CODE_GENERATOR_H
    #define CODE_GENERATOR_H

    #include <string>
    #include <vector>
    #include <ast/node.h>
    #include <base/object_ref.h>

    namespace Generation
    {

        Base::ObjectRef<AST::TranslationUnit> GenerateShader(
            const std::vector<std::string> & fragment_name_table,
            const std::vector<std::string> & semantic_table
            );
    }


#endif