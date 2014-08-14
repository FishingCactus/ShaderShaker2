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
    bool CodeGenerator::FindMatchingFunction(
        FunctionDefinition::Ref & function,
        std::set<FunctionDefinition::Ref> & used_function_set,
        const std::set<std::string> & semantic_set,
        const std::vector<FragmentDefinition::Ref > & definition_table
        )
    {
        std::vector<FragmentDefinition::Ref >::const_reverse_iterator it, end;

        it = definition_table.rbegin();
        end = definition_table.rend();

        for(; it!=end; ++it )
        {
            if( (*it)->FindFunctionDefinitionMatchingSemanticSet( function, semantic_set ) )
            {
                if( used_function_set.find( function ) != used_function_set.end() )
                {
                    continue;
                }

                return true;
            }
        }

        return false;
    }

    void CodeGenerator::RemoveInputSemantics( std::set<std::string> & semantic_set )
    {
        std::set<std::string> new_semantic_set;

        std::set_intersection(
            semantic_set.begin(), semantic_set.end(),
            m_InputSemanticSet.begin(), m_InputSemanticSet.end(),
            std::inserter( m_UsedSemanticSet, m_UsedSemanticSet.begin() )
            );

        std::set_difference(
            semantic_set.begin(), semantic_set.end(),
            m_InputSemanticSet.begin(), m_InputSemanticSet.end(),
            std::inserter( new_semantic_set, new_semantic_set.begin() )
            );

        semantic_set = std::move( new_semantic_set );
    }

    Base::ObjectRef<AST::Statement> GetFunctionCallFromFunctionDefinition(
        const FunctionDefinition & definition
        )
    {
        const AST::FunctionDeclaration & function_declaration = definition.GetFunctionDeclaration();
        Base::ObjectRef<AST::ArgumentExpressionList> argument_expression_list = new AST::ArgumentExpressionList;

        if( function_declaration.m_ArgumentList )
        {
            const AST::ArgumentList & argument_list = *function_declaration.m_ArgumentList;
            std::vector< Base::ObjectRef<AST::Argument> >::const_iterator it, end;
            it = argument_list.m_ArgumentTable.begin();
            end = argument_list.m_ArgumentTable.end();

            for(;it != end; ++it )
            {
                argument_expression_list->m_ExpressionList.push_back( new AST::VariableExpression( (*it)->m_Semantic ) );
            }
        }

        Base::ObjectRef<AST::CallExpression> call_expression = new AST::CallExpression( function_declaration.m_Name, &*argument_expression_list );

        if( function_declaration.m_Semantic.empty() )
        {
            return new AST::ExpressionStatement( &*call_expression );
        }
        else
        {
            return new AST::AssignmentStatement(
                new AST::LValueExpression( new AST::VariableExpression( function_declaration.m_Semantic ) ),
                AST::AssignmentOperator_Assign,
                &*call_expression
                );
        }
    }

    struct CodeGeneratorHelper
    {
        void Visit( const GraphNode & node )
        {
            if( m_VisitedNodeSet.find( &node ) != m_VisitedNodeSet.end() )
            {
                return;
            }

            if( !node.HasFunctionDefinition() )
            {
                return;
            }

            m_VisitedNodeSet.insert( &node );

            std::set<std::string>::iterator it, end;
            it = node.GetFunctionDefinition().GetOutSemanticSet().begin();
            end = node.GetFunctionDefinition().GetOutSemanticSet().end();

            for(; it != end; ++it )
            {
                if( m_DeclaredVariableTable.find( *it ) == m_DeclaredVariableTable.end() )
                {
                    m_DeclaredVariableTable.insert( *it );

                    //:TODO: get semantic type

                    Base::ObjectRef<AST::VariableDeclarationStatement> variable = new AST::VariableDeclarationStatement;

                    variable->SetType( new AST::Type( "float4" ) );
                    variable->AddBody( new AST::VariableDeclarationBody( *it ) );

                    m_StatementTable.push_back( &*variable );

                }
            }

            m_StatementTable.push_back( GetFunctionCallFromFunctionDefinition( node.GetFunctionDefinition() ) );

        }

        std::set<std::string>
            m_DeclaredVariableTable;
        std::set<const GraphNode *>
            m_VisitedNodeSet;
        std::vector<Base::ObjectRef<AST::Statement> >
            m_StatementTable;

    };

    Base::ObjectRef<AST::FunctionDeclaration> CodeGenerator::GenerateCodeFromGraph(
        const Graph & graph
        )
    {
        CodeGeneratorHelper helper;

        helper.m_DeclaredVariableTable.insert( m_UsedSemanticSet.begin(), m_UsedSemanticSet.end() );
        helper.m_DeclaredVariableTable.insert( m_OutputSemanticSet.begin(), m_OutputSemanticSet.end());

        graph.VisitDepthFirst( helper );

        return 0;
    }

    Graph::Ref CodeGenerator::GenerateGraph(
        const std::vector<FragmentDefinition::Ref > & fragment_table
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

        open_set.insert( m_OutputSemanticSet.begin(), m_OutputSemanticSet.end() );

        graph = new Graph;
        graph->Initialize( open_set );

        while( !open_set.empty() )
        {
            std::set<std::string> new_open_set;
            if( !FindMatchingFunction( function, used_function_set, open_set, fragment_table ) )
            {
                std::ostream_iterator< std::string > output( std::cerr, ", " );

                std::cerr << "Unable to find function that generates ";
                std::copy( open_set.begin(), open_set.end(), output );
                return 0;
            }

            GraphNode::Ref node = new GraphNode( *function );
            used_function_set.insert( function );

            std::set_difference(
                open_set.begin(), open_set.end(),
                function->GetOutSemanticSet().begin(), function->GetOutSemanticSet().end(),
                std::inserter(new_open_set, new_open_set.begin() )
                );

            open_set = std::move( new_open_set );

            closed_set.insert( function->GetOutSemanticSet().begin(), function->GetOutSemanticSet().end() );
            open_set.insert( function->GetInSemanticSet().begin(), function->GetInSemanticSet().end() );
            open_set.insert( function->GetInOutSemanticSet().begin(), function->GetInOutSemanticSet().end() );

            if( !graph->AddNode( *node ) )
            {
                return 0;
            }

            RemoveInputSemantics( open_set );
        }

        return graph;
    }

    Base::ObjectRef<AST::TranslationUnit> CodeGenerator::GenerateShader(
        const std::vector<std::string> & fragment_name_table,
        const std::vector<std::string> & semantic_table,
        const std::vector<std::string> & semantic_input_table
        )
    {
        std::vector<Base::ObjectRef<FragmentDefinition> >
            definition_table;

        m_OutputSemanticSet.clear();
        m_InputSemanticSet.clear();
        m_InputSemanticSet.insert( semantic_input_table.begin(), semantic_input_table.end() );
        m_OutputSemanticSet.insert( semantic_table.begin(), semantic_table.end() );

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

        Graph::Ref graph = GenerateGraph( definition_table );

        Base::ObjectRef<AST::FunctionDeclaration> function = GenerateCodeFromGraph( *graph );

        return 0;
    }

}