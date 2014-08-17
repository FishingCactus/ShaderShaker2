#include "function_definition.h"
#include <ast/empty_visitor.h>
#include <ast/node.h>
#include <iostream>

namespace  Generation
{

    void FunctionDefinition::FillFromFunctionDeclaration(
        AST::FunctionDeclaration & declaration
        )
    {
        m_Name = declaration.m_Name;
        m_FunctionDeclaration = &declaration;

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

                SetTypeForSemantic( argument.m_Type->m_Name, argument.m_Semantic );
            }
        }

        if( !declaration.m_Semantic.empty() )
        {
            m_OutSemanticSet.insert( declaration.m_Semantic );

            SetTypeForSemantic( declaration.m_Type->m_Name, declaration.m_Semantic );
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

    std::string FunctionDefinition::GetSemanticType( const std::string & semantic ) const
    {
        std::map< std::string, std::string >::const_iterator it;

        it = m_SemanticToTypeMap.find( semantic );

        if( it != m_SemanticToTypeMap.end() )
        {
            return (*it).second;
        }
        else
        {
            assert( "Unable to find type for semantic" );
            return "";
        }
    }

    void FunctionDefinition::SetTypeForSemantic(
        const std::string & type,
        const std::string & semantic
        )
    {
        std::map<std::string, std::string>::iterator it;

        it = m_SemanticToTypeMap.find( semantic );

        if( it == m_SemanticToTypeMap.end() )
        {
            m_SemanticToTypeMap.insert( make_pair( semantic, type ) );
        }
        else
        {
            if( (*it).second != type )
            {
                std::cerr << "Incompatible type for semantic " << semantic
                    << " in fragment " << m_Name << ": appears with type "
                    << (*it).second << " and type " << type;
            }
        }
    }
}