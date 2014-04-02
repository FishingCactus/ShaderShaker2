#include "print_visitor.h"

#include <assert.h>
#include <iostream>
#include "ast/node.h"
#include "utils/indentation.h"

namespace AST
{

    template< class _Table_ >
    void VisitTable( Visitor & visitor, _Table_ & table )
    {
        typename _Table_::iterator
            it = table.begin(),
            end = table.end();

        for( ; it != end; ++it )
        {
            (*it)->Visit( visitor );
        }
    }

    void PrintVisitor::Visit( Node & node )
    {
        assert(! "Some node type might not be supported in the visitor" );

    }

    void PrintVisitor::Visit( TranslationUnit & translation_unit )
    {

        std::cerr
            << "TranslationUnit" << endl_ind
            << "{" << inc_ind << endl_ind;

        VisitTable( *this, translation_unit.m_GlobalDeclarationTable );
        VisitTable( *this, translation_unit.m_TechniqueTable );

        std::cerr << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( VariableDeclaration & variable_declaration )
    {
        std::cerr
            << "VariableDeclaration" << endl_ind
            << "{" << inc_ind << endl_ind;

        variable_declaration.m_Type->Visit( *this );

        VisitTable( *this, variable_declaration.m_StorageClass );
        VisitTable( *this, variable_declaration.m_TypeModifier );

        VisitTable( *this, variable_declaration.m_BodyTable );

        std::cerr << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( IntrinsicType & type )
    {
        std::cerr << "IntrinsicType{ " << type.m_Name << " }" << endl_ind;

    }

    void PrintVisitor::Visit( UserDefinedType & type )
    {
        std::cerr << "UserDefinedType{ " << type.m_Name << " }" << endl_ind;

    }

    void PrintVisitor::Visit( SamplerType & type )
    {
        std::cerr << "SamplerType{ " << type.m_Name << " }" << endl_ind;
    }

    void PrintVisitor::Visit( TypeModifier & modifier )
    {
        std::cerr << "TypeModifier{ " << modifier.m_Value << " }" << endl_ind;

    }

    void PrintVisitor::Visit( StorageClass & storage_class )
    {
        std::cerr << "StorageClass{ " << storage_class.m_Value << " }" << endl_ind;
    }

    void PrintVisitor::Visit( VariableDeclarationBody & body )
    {

        std::cerr
            << "VariableDeclarationBody" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << body.m_Name << " }" << endl_ind;

        if( !body.m_Semantic.empty() )
        {
            std::cerr << "Semantic{ " << body.m_Semantic << " }" << endl_ind;
        }

        if( body.m_InitialValue )
        {
            std::cerr << "InitialValue{ " << body.m_InitialValue << " }" << endl_ind;
        }

        if( body.m_ArraySize != 0 )
        {
            std::cerr << "ArraySize{ " << body.m_ArraySize << " }" << endl_ind;
        }

        std::cerr << dec_ind << endl_ind << "}" << endl_ind;

    }

    void PrintVisitor::Visit( TextureDeclaration & declaration )
    {
        std::cerr
            << "TextureDeclaration" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Type{ " << declaration.m_Type << " }" << endl_ind
            << "Name{ " << declaration.m_Name << " }" << endl_ind
            << "Semantic{ " << declaration.m_Semantic << " }" << endl_ind;

        if( declaration.m_Annotations )
        {
            declaration.m_Annotations->Visit( *this );
        }

        std::cerr << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( SamplerDeclaration & declaration )
    {
        std::cerr
            << "SamplerDeclaration" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Type{ " << declaration.m_Type << " }" << endl_ind
            << "Name{ " << declaration.m_Name << " }" << endl_ind;

        VisitTable( *this, declaration.m_BodyTable );

        std::cerr << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( SamplerBody & body )
    {
        std::cerr
            << "SamplerBody" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << body.m_Name << " }" << endl_ind
            << "Value{ " << body.m_Value << " }" << endl_ind
            << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( StructDefinition & definition )
    {
        std::cerr
            << "StructDefinition" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << definition.m_Name << " }" << endl_ind;

        std::vector<StructDefinition::Member>::iterator it, end = definition.m_MemberTable.end();

        for( it = definition.m_MemberTable.begin(); it != end; ++it )
        {
            std::cerr
                << "Member" << endl_ind
                << "{ " << inc_ind << endl_ind
                << "Name{ " << (*it).m_Name << " }" << endl_ind;

            (*it).m_Type->Visit( *this );

            std::cerr
                << "Semantic{ " << (*it).m_Semantic << " }" << endl_ind
                << "InterpolationModifier{" << (*it).m_InterpolationModifier << " }" << endl_ind
                << dec_ind << endl_ind << "}" << endl_ind;
        }

        std::cerr << dec_ind << endl_ind << "}" << endl_ind;
    }
};