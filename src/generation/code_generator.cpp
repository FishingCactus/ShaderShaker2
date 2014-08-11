#include "code_generator.h"
#include <hlsl_parser/hlsl.h>
#include <base/object_ref.h>
#include "fragment_definition.h"
#include "function_definition.h"
#include "graph.h"
#include "graph_node.h"
#include <set>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace Generation
{
    bool FindMatchingFunction(
        FunctionDefinition::Ref & function,
        std::set<FunctionDefinition::Ref> & used_function_set,
        const std::set<std::string> & semantic_set,
        const std::vector<FragmentDefinition::Ref > & definition
        )
    {
        return false;
    }

    void RemoveInputSemantics( std::set<std::string> & semantic_set )
    {
    }

    Graph::Ref GenerateGraph(
        const std::vector<FragmentDefinition::Ref > & fragment_table,
        const std::vector<std::string> & semantic_table
        )
    {
        std::set<std::string>
            open_set,
            closed_set;
        Graph::Ref
            graph;
        FunctionDefinition::Ref
            function;
        std::set<FunctionDefinition::Ref>
            used_function_set;

        graph = new Graph;

        open_set.insert( semantic_table.begin(), semantic_table.end() );

        while( !open_set.empty() )
        {
            if( !FindMatchingFunction( function, used_function_set, open_set, fragment_table ) )
            {
                std::ostream_iterator< std::string > output( std::cerr, ", " );

                std::cerr << "Unable to find function that generates ";
                std::copy( open_set.begin(), open_set.end(), output );
                return 0;
            }

            GraphNode::Ref node = new GraphNode( *function );

            if( !graph->AddNode( *node ) )
            {
                return 0;
            }

            RemoveInputSemantics( open_set );
        }

        return graph;
    }

    Base::ObjectRef<AST::TranslationUnit> GenerateShader(
        const std::vector<std::string> & fragment_name_table,
        const std::vector<std::string> & semantic_table
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

        GenerateGraph( definition_table, semantic_table );

        return 0;
    }

}