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

    Annotations * Annotations::Clone() const
    {
        Annotations * clone = new Annotations;

        CloneTable( AnnotationEntry, m_AnnotationTable )

        return clone;
    }

    AnnotationEntry * AnnotationEntry::Clone() const
    {
        AnnotationEntry * clone = new AnnotationEntry;

        clone->m_Type = m_Type;
        clone->m_Name = m_Name;
        clone->m_Value = m_Value;

        return clone;
    }
}