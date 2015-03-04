#include "code_generator.h"
#include <base/object_ref.h>
#include "fragment_definition.h"
#include "function_definition.h"
#include "graph.h"
#include "graph_node.h"
#include "graph_validator.h"
#include <ast/function_node.h>
#include "semantic_remover.h"
#include <set>
#include <iostream>
#include <iterator>
#include <sstream>
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

                //:TRICKY: This is a set, but order should be deterministic
                if ( std::find( m_UsedTranslationUnitSet.begin(), m_UsedTranslationUnitSet.end(), &(*it)->GetTranslationUnit() )
                        == m_UsedTranslationUnitSet.end()
                    )
                {
                    m_UsedTranslationUnitSet.push_back( const_cast<AST::TranslationUnit*>( &(*it)->GetTranslationUnit() ) );
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

                    Base::ObjectRef<AST::VariableDeclarationStatement> variable = new AST::VariableDeclarationStatement;

                    variable->SetType( new AST::Type( node.GetFunctionDefinition().GetSemanticType( *it ) ) );
                    variable->AddBody( new AST::VariableDeclarationBody( *it ) );

                    m_StatementTable.push_back( &*variable );

                }
            }

            m_StatementTable.push_back( GetFunctionCallFromFunctionDefinition( node.GetFunctionDefinition() ) );

            InsertSemanticTypes( node.GetFunctionDefinition().GetSemanticTypeTable() );
        }

        void InsertSemanticTypes( const std::map<std::string, std::string> & semantic_to_type_table )
        {
            std::map<std::string, std::string>::const_iterator it, end;

            it = semantic_to_type_table.begin();
            end = semantic_to_type_table.end();

            for( ; it!=end; ++it )
            {
                std::map<std::string, std::string>::iterator value;
                value = m_SemanticToTypeTable.find( (*it).first );

                if( value != m_SemanticToTypeTable.end() )
                {
                    if( (*value).second != (*it).second )
                    {
                        std::cerr << "Different types can be found for semantic " << (*value).first
                            << ": " << (*it).second << " and " << (*value).second;
                    }
                }
                else
                {
                    m_SemanticToTypeTable.insert( (*it) );
                }

            }
        }

        std::set<std::string>
            m_DeclaredVariableTable;
        std::set<const GraphNode *>
            m_VisitedNodeSet;
        std::vector<Base::ObjectRef<AST::Statement> >
            m_StatementTable;
        std::map<std::string, std::string>
            m_SemanticToTypeTable;
    };

    Base::ObjectRef<AST::FunctionDeclaration> CodeGenerator::GenerateCodeFromGraph(
        const Graph & graph
        )
    {
        CodeGeneratorHelper helper;

        helper.m_DeclaredVariableTable.insert( m_UsedSemanticSet.begin(), m_UsedSemanticSet.end() );
        helper.m_DeclaredVariableTable.insert( m_OutputSemanticSet.begin(), m_OutputSemanticSet.end());
        graph.VisitDepthFirst( helper );

        Base::ObjectRef<AST::ArgumentList> argument_list = new AST::ArgumentList;

        std::set<std::string>::const_iterator it,end;

        it = m_OutputSemanticSet.begin();
        end = m_OutputSemanticSet.end();
        for(;it!=end;++it )
        {
            Base::ObjectRef<AST::Argument> argument = new AST::Argument;
            argument->m_Type = new AST::Type( helper.m_SemanticToTypeTable.find( (*it) )->second );
            argument->m_Name = (*it);
            argument->m_Semantic = (*it);
            argument->m_InputModifier = "out";

            argument_list->m_ArgumentTable.push_back( argument );
        }

        it = m_InputSemanticSet.begin();
        end = m_InputSemanticSet.end();
        for(;it!=end;++it )
        {
            Base::ObjectRef<AST::Argument> argument = new AST::Argument;
            argument->m_Type = new AST::Type( helper.m_SemanticToTypeTable.find( (*it) )->second ); //:TODO: get real type;
            argument->m_Name = (*it);
            argument->m_Semantic = (*it);
            argument->m_InputModifier = "in";

            argument_list->m_ArgumentTable.push_back( argument );
        }

        Base::ObjectRef<AST::FunctionDeclaration> function_declaration = new AST::FunctionDeclaration;

        function_declaration->m_Name = "main";
        function_declaration->m_StatementTable = std::move( helper.m_StatementTable );
        function_declaration->m_ArgumentList = argument_list;

        return function_declaration;
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
                std::ostringstream message;

                std::ostream_iterator< std::string > output( message, ", " );
                message << "Unable to find function that generates ";
                std::copy( open_set.begin(), open_set.end(), output );

                m_ErrorHandler->ReportError( message.str(), "" );

                return 0;
            }

            GraphNode::Ref node = new GraphNode( *function );
            used_function_set.insert( function );

            if( !graph->AddNode( *node ) )
            {
                return 0;
            }

            // Bind to already existing semantic
            std::set<std::string>::iterator it, end;
            std::set<std::string> unresolved_semantic;

            it = function->GetInSemanticSet().begin();
            end = function->GetInSemanticSet().end();

            for( ; it != end; ++it )
            {
                if( open_set.find( *it ) == open_set.end()
                    && graph->HasGeneratedSemantic( *it ) )
                {
                    if( !graph->UseGeneratedSemantic( *node, *it ) )
                    {
                        m_ErrorHandler->ReportError( "Cycle detected involving " + *it, "" );
                        return 0;
                    }
                }
                else
                {
                    unresolved_semantic.insert( *it );
                }
            }

            std::set_difference(
                open_set.begin(), open_set.end(),
                function->GetOutSemanticSet().begin(), function->GetOutSemanticSet().end(),
                std::inserter(new_open_set, new_open_set.begin() )
                );

            open_set = std::move( new_open_set );

            closed_set.insert( function->GetOutSemanticSet().begin(), function->GetOutSemanticSet().end() );
            open_set.insert( unresolved_semantic.begin(), unresolved_semantic.end() );
            open_set.insert( function->GetInOutSemanticSet().begin(), function->GetInOutSemanticSet().end() );

            RemoveInputSemantics( open_set );
        }

        return graph;
    }

    void CodeGenerator::GenerateShader(
        Base::ObjectRef<AST::TranslationUnit> & generated_shader,
        std::vector<std::string> & used_semantic_set,
        const std::vector<Base::ObjectRef<FragmentDefinition> > & definition_table,
        const std::vector<std::string> & semantic_table,
        const std::vector<std::string> & semantic_input_table,
        Base::ErrorHandlerInterface & error_handler
        )
    {
        m_ErrorHandler = & error_handler;
        m_UsedTranslationUnitSet.clear();
        m_UsedSemanticSet.clear();
        m_OutputSemanticSet.clear();
        m_InputSemanticSet.clear();
        m_InputSemanticSet.insert( semantic_input_table.begin(), semantic_input_table.end() );
        m_OutputSemanticSet.insert( semantic_table.begin(), semantic_table.end() );

        Graph::Ref graph = GenerateGraph( definition_table );

        if( !graph )
        {
            return;
        }

        if( !ValidatesGraph( *graph ) )
        {
            return;
        }

        Base::ObjectRef<AST::FunctionDeclaration> function = GenerateCodeFromGraph( *graph );

        if( !function )
        {
            return;
        }

        Base::ObjectRef<AST::TranslationUnit> translation_unit = new AST::TranslationUnit;

        MergeTranslationUnit( *translation_unit, m_UsedTranslationUnitSet );

        translation_unit->m_GlobalDeclarationTable.push_back( &*function );

        generated_shader = translation_unit;
        std::copy( m_UsedSemanticSet.begin(), m_UsedSemanticSet.end(), std::back_inserter( used_semantic_set ) );
    }

    void CodeGenerator::MergeTranslationUnit(
        AST::TranslationUnit & destination_translation_unit,
        const std::vector<Base::ObjectRef<AST::TranslationUnit> > & translation_unit_table
        )
    {
        std::vector<Base::ObjectRef<AST::TranslationUnit> >::const_iterator it, end;
        SemanticRemover semantic_remover;

        it = translation_unit_table.begin();
        end = translation_unit_table.end();
        for( ;it!=end; ++it)
        {
            Base::ObjectRef<AST::TranslationUnit> clone = (*it)->Clone();

            clone->Visit( semantic_remover );

            std::copy(
                clone->m_GlobalDeclarationTable.begin(), clone->m_GlobalDeclarationTable.end(),
                std::back_inserter( destination_translation_unit.m_GlobalDeclarationTable )
                );
        }

    }

    bool CodeGenerator::ValidatesGraph(
        const Graph & graph
        ) const
    {
        GraphValidator
            validator( *m_ErrorHandler );

        graph.VisitDepthFirst( validator );

        return !validator.HasErrors();
    }


}