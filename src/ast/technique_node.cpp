#include "node.h"

namespace AST
{
    #define CloneTable( _Type_, _member_ ) \
    { \
        std::vector< Base::ObjectRef<_Type_> >::const_iterator it, end; \
        \
        it = _member_.begin(); \
        end = _member_.end(); \
        for( ; it!=end; ++it ) \
        { \
            clone->_member_.push_back( (*it)->Clone() ); \
        } \
    }

    Technique * Technique::Clone() const
    {
        Technique * clone = new Technique;

        clone->m_Name = m_Name;
        CloneTable( Pass, m_PassTable )

        return clone;
    }

    Pass * Pass::Clone() const
    {
        Pass * clone = new Pass;

        clone->m_Name = m_Name;
        CloneTable( ShaderDefinition, m_ShaderDefinitionTable )

        return clone;
    }

    ShaderDefinition * ShaderDefinition::Clone() const
    {
        ShaderDefinition * clone = new ShaderDefinition;

        clone->m_Name = m_Name;
        clone->m_Type = m_Type;
        clone->m_List = m_List->Clone();

        return clone;
    }

    ShaderArgumentList * ShaderArgumentList::Clone() const
    {
        ShaderArgumentList * clone = new ShaderArgumentList;

        CloneTable( Expression, m_ShaderArgumentTable );

        return clone;
    }
}