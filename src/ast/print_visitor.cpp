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

};