#include "function_definition.h"
#include "ast/empty_visitor.h"
#include "ast/node.h"

namespace  Generation
{

    void FunctionDefinition::FillFromFunctionDeclaration(
        AST::FunctionDeclaration & declaration
        )
    {
        m_Name = declaration.m_Name;

        if( declaration.m_ArgumentList )
        {
            std::vector< Base::ObjectRef<AST::Argument> >
                & argument_table = declaration.m_ArgumentList->m_ArgumentTable;

            std::vector< Base::ObjectRef<AST::Argument> >::const_iterator it, end;

            it = argument_table.begin();
            end = argument_table.end();

            for(;it!=end;++it)
            {
                const AST::Argument & argument = **it;

                if( argument.m_InputModifier == "out" )
                {
                    m_OutSemanticSet.insert( argument.m_Semantic );
                }
                else if( argument.m_InputModifier == "inout" )
                {
                    m_InOutSemanticSet.insert( argument.m_Semantic );
                }
                else
                {
                    m_InSemanticSet.insert( argument.m_Semantic );
                }
            }
        }

        if( !declaration.m_Semantic.empty() )
        {
            m_OutSemanticSet.insert( declaration.m_Semantic );
        }
    }

    void FunctionDefinition::GetAllOutSemanticSet( std::set<std::string> & set)
    {
        set.insert( m_OutSemanticSet.begin(), m_OutSemanticSet.end() );
        set.insert( m_InOutSemanticSet.begin(), m_InOutSemanticSet.end() );
    }

    void FunctionDefinition::GetAllInSemanticSet( std::set<std::string> & set)
    {
        set.insert( m_InSemanticSet.begin(), m_InSemanticSet.end() );
        set.insert( m_InOutSemanticSet.begin(), m_InOutSemanticSet.end() );
    }
}