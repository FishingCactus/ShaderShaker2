#include "code_generator.h"
#include <hlsl_parser/hlsl.h>
#include "fragment_definition.h"

namespace Generation
{

    Base::ObjectRef<AST::TranslationUnit> GenerateShader(
        const std::vector<std::string> & fragment_name_table
        )
    {
        std::vector<Base::ObjectRef<FragmentDefinition> >
            definition_table;

        std::vector<std::string>::const_iterator it, end;
        it = fragment_name_table.begin();
        end = fragment_name_table.end();

        for(; it!=end; ++it )
        {
            Base::ObjectRef<AST::TranslationUnit>
                translation_unit;
            Base::ObjectRef<Generation::FragmentDefinition>
                definition;

            translation_unit = HLSL::ParseHLSL( (*it).c_str() );

            FragmentDefinition::GenerateFragment( definition, *translation_unit );

            definition_table.push_back( definition );
        }

        return 0;
    }

}