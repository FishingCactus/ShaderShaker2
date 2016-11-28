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

    Node::Node():
        m_FileName( s_CurrentFileName ),
        m_Line( s_CurrentLine )
    {

    }

    void Node::SetDebugInfo(
        const std::string & filename,
        const int line
        )
    {
        s_CurrentFileName = filename;
        s_CurrentLine = line;
    }

    std::string
        Node::s_CurrentFileName;
    int
        Node::s_CurrentLine = -1;


    TranslationUnit * TranslationUnit::Clone() const
    {
        TranslationUnit * clone = new TranslationUnit;

        CloneTable( GlobalDeclaration, m_GlobalDeclarationTable )
        CloneTable( Technique, m_TechniqueTable )

        return clone;
    }

    VariableDeclaration * VariableDeclaration::Clone() const
    {
        VariableDeclaration * clone = new VariableDeclaration;

        clone->m_Type = m_Type->Clone();
        CloneTable( StorageClass, m_StorageClass )
        CloneTable( TypeModifier, m_TypeModifier )
        CloneTable( VariableDeclarationBody, m_BodyTable )

        return clone;
    }

    TextureDeclaration * TextureDeclaration::Clone() const
    {
        TextureDeclaration * clone = new TextureDeclaration;
        clone->m_Type = m_Type;
        clone->m_Name = m_Name;
        clone->m_Semantic = m_Semantic;
        clone->m_Annotations = m_Annotations->Clone();

        return clone;
    }


    SamplerDeclaration * SamplerDeclaration::Clone() const
    {
        SamplerDeclaration * clone = new SamplerDeclaration;

        clone->m_Type = m_Type;
        clone->m_Name = m_Name;
        CloneTable( SamplerBody, m_BodyTable );

        return clone;
    }

    SamplerBody * SamplerBody::Clone() const
    {
        SamplerBody * clone = new SamplerBody;

        clone->m_Name = m_Name;
        clone->m_Value = m_Value;

        return clone;
    }

    StructDefinition * StructDefinition::Clone() const
    {
        StructDefinition * clone = new StructDefinition;

        clone->m_Name = m_Name;
        clone->m_MemberTable = m_MemberTable;

        std::vector<Member>::iterator it, end;

        it = clone->m_MemberTable.begin();
        end = clone->m_MemberTable.end();

        for( ; it != end; ++it )
        {
            (*it).m_Type = (*it).m_Type->Clone();
        }

        return clone;
    }

    IntrinsicType * IntrinsicType::Clone() const
    {
        IntrinsicType * clone = new IntrinsicType;

        clone->m_Name = m_Name;

        return clone;
    }

    UserDefinedType * UserDefinedType::Clone() const
    {
        UserDefinedType * clone = new UserDefinedType;

        clone->m_Name = m_Name;

        return clone;
    }

    SamplerType * SamplerType::Clone() const
    {
        SamplerType * clone = new SamplerType;

        clone->m_Name = m_Name;

        return clone;
    }

    TypeModifier * TypeModifier::Clone() const
    {
        TypeModifier * clone = new TypeModifier;

        clone->m_Value = m_Value;

        return clone;
    }

    StorageClass * StorageClass::Clone() const
    {
        StorageClass * clone = new StorageClass;

        clone->m_Value = m_Value;

        return clone;
    }

    VariableDeclarationBody * VariableDeclarationBody::Clone() const
    {
        VariableDeclarationBody * clone = new VariableDeclarationBody;

        clone->m_Name = m_Name;
        clone->m_Semantic = m_Semantic;
        if( m_InitialValue )
        {
            clone->m_InitialValue = m_InitialValue->Clone();
        }

        clone->m_ArraySize = m_ArraySize;

        if ( m_Annotations )
        {
            clone->m_Annotations = m_Annotations->Clone();
        }

        return clone;
    }

    InitialValue * InitialValue::Clone() const
    {
        InitialValue * clone = new InitialValue;

        CloneTable( Expression, m_ExpressionTable );

        clone->m_Vector = m_Vector;

        return clone;
    }
}