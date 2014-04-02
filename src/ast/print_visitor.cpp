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
        std::cout
            << "TranslationUnit" << endl_ind
            << "{" << inc_ind << endl_ind;

        VisitTable( *this, translation_unit.m_GlobalDeclarationTable );
        VisitTable( *this, translation_unit.m_TechniqueTable );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( VariableDeclaration & variable_declaration )
    {
        std::cout
            << "VariableDeclaration" << endl_ind
            << "{" << inc_ind << endl_ind;

        variable_declaration.m_Type->Visit( *this );

        VisitTable( *this, variable_declaration.m_StorageClass );
        VisitTable( *this, variable_declaration.m_TypeModifier );

        VisitTable( *this, variable_declaration.m_BodyTable );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( IntrinsicType & type )
    {
        std::cout << "IntrinsicType{ " << type.m_Name << " }" << endl_ind;

    }

    void PrintVisitor::Visit( UserDefinedType & type )
    {
        std::cout << "UserDefinedType{ " << type.m_Name << " }" << endl_ind;

    }

    void PrintVisitor::Visit( SamplerType & type )
    {
        std::cout << "SamplerType{ " << type.m_Name << " }" << endl_ind;
    }

    void PrintVisitor::Visit( TypeModifier & modifier )
    {
        std::cout << "TypeModifier{ " << modifier.m_Value << " }" << endl_ind;

    }

    void PrintVisitor::Visit( StorageClass & storage_class )
    {
        std::cout << "StorageClass{ " << storage_class.m_Value << " }" << endl_ind;
    }

    void PrintVisitor::Visit( VariableDeclarationBody & body )
    {

        std::cout
            << "VariableDeclarationBody" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << body.m_Name << " }" << endl_ind;

        if( !body.m_Semantic.empty() )
        {
            std::cout << "Semantic{ " << body.m_Semantic << " }" << endl_ind;
        }

        if( body.m_InitialValue )
        {
            std::cout << "InitialValue{ " << body.m_InitialValue << " }" << endl_ind;
        }

        if( body.m_ArraySize != 0 )
        {
            std::cout << "ArraySize{ " << body.m_ArraySize << " }" << endl_ind;
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;

    }

    void PrintVisitor::Visit( TextureDeclaration & declaration )
    {
        std::cout
            << "TextureDeclaration" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Type{ " << declaration.m_Type << " }" << endl_ind
            << "Name{ " << declaration.m_Name << " }" << endl_ind
            << "Semantic{ " << declaration.m_Semantic << " }" << endl_ind;

        if( declaration.m_Annotations )
        {
            declaration.m_Annotations->Visit( *this );
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( SamplerDeclaration & declaration )
    {
        std::cout
            << "SamplerDeclaration" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Type{ " << declaration.m_Type << " }" << endl_ind
            << "Name{ " << declaration.m_Name << " }" << endl_ind;

        VisitTable( *this, declaration.m_BodyTable );

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( SamplerBody & body )
    {
        std::cout
            << "SamplerBody" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << body.m_Name << " }" << endl_ind
            << "Value{ " << body.m_Value << " }" << endl_ind
            << dec_ind << endl_ind << "}" << endl_ind;
    }

    void PrintVisitor::Visit( StructDefinition & definition )
    {
        std::cout
            << "StructDefinition" << endl_ind
            << "{ " << inc_ind << endl_ind
            << "Name{ " << definition.m_Name << " }" << endl_ind;

        std::vector<StructDefinition::Member>::iterator it, end = definition.m_MemberTable.end();

        for( it = definition.m_MemberTable.begin(); it != end; ++it )
        {
            std::cout
                << "Member" << endl_ind
                << "{ " << inc_ind << endl_ind
                << "Name{ " << (*it).m_Name << " }" << endl_ind;

            (*it).m_Type->Visit( *this );

            std::cout
                << "Semantic{ " << (*it).m_Semantic << " }" << endl_ind
                << "InterpolationModifier{" << (*it).m_InterpolationModifier << " }" << endl_ind
                << dec_ind << endl_ind << "}" << endl_ind;
        }

        std::cout << dec_ind << endl_ind << "}" << endl_ind;
    }
};