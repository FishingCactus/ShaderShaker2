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

    FunctionDeclaration * FunctionDeclaration::Clone() const
    {
        FunctionDeclaration * clone = new FunctionDeclaration;

        clone->m_Type = m_Type->Clone();
        clone->m_Name = m_Name;
        clone->m_Semantic = m_Semantic;
        clone->m_ArgumentList = m_ArgumentList->Clone();
        CloneTable( StorageClass, m_StorageClassTable );
        CloneTable( Statement, m_StatementTable );

        return clone;
    }

    ArgumentList * ArgumentList::Clone() const
    {
        ArgumentList * clone = new ArgumentList;

        CloneTable( Argument, m_ArgumentTable )

        return clone;
    }

    Argument * Argument::Clone() const
    {
        Argument * clone = new Argument;

        clone->m_Type = m_Type->Clone();
        clone->m_Name = m_Name;
        clone->m_InputModifier = m_InputModifier;
        clone->m_Semantic = m_Semantic;
        clone->m_InterpolationModifier = m_InterpolationModifier;
        clone->m_TypeModifier = m_TypeModifier->Clone();
        clone->m_InitialValue = m_InitialValue->Clone();

        return clone;
    }
}