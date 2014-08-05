#include "fragment_definition.h"
#include "function_definition.h"
#include "ast/empty_visitor.h"
#include "ast/node.h"

namespace Generation
{

	class GetFunctionVisitor : public AST::EmptyVisitor
	{
    public:

        std::vector<Base::ObjectRef<AST::FunctionDeclaration> >::iterator begin()
        {
            return m_FunctionDeclarationTable.begin();
        }

        std::vector<Base::ObjectRef<AST::FunctionDeclaration> >::iterator end()
        {
            return m_FunctionDeclarationTable.end();
        }

    private:
        virtual void Visit( AST::TranslationUnit & translation_unit ) override
        {
            std::vector< Base::ObjectRef<AST::GlobalDeclaration> >::iterator it,end;
            it = translation_unit.m_GlobalDeclarationTable.begin();
            end = translation_unit.m_GlobalDeclarationTable.end();

            for( ;it != end; ++it )
            {
                (*it)->Visit( *this );
            }
        }

        virtual void Visit( AST::FunctionDeclaration & function_declaration ) override
        {
            m_FunctionDeclarationTable.emplace_back( &function_declaration );
        }

        std::vector<Base::ObjectRef<AST::FunctionDeclaration> >
            m_FunctionDeclarationTable;
	};

	void FragmentDefinition::GenerateFragment(
		Base::ObjectRef<FragmentDefinition> & fragment_definition,
		AST::TranslationUnit & translation_unit
		)
	{
		GetFunctionVisitor
			visitor;

		translation_unit.Visit( visitor );

        fragment_definition = new FragmentDefinition;

        fragment_definition->m_AbstractSyntaxTree = &translation_unit;

        std::vector<Base::ObjectRef<AST::FunctionDeclaration> >::iterator it, end;

        for( it = visitor.begin(), end = visitor.end(); it != end; ++it )
        {
            Base::ObjectRef<FunctionDefinition> function_definition( new FunctionDefinition );

            function_definition->FillFromFunctionDeclaration( **it );

            fragment_definition->m_FunctionDefinitionTable.push_back(function_definition);
        }

	}

    bool FragmentDefinition::FindFunctionDefinition(
        Base::ObjectRef<FunctionDefinition> & definition,
        const std::string & name
        )
    {
        std::vector<Base::ObjectRef<FunctionDefinition> >::iterator it, end;

        for( it = m_FunctionDefinitionTable.begin(), end = m_FunctionDefinitionTable.end(); it != end; ++it )
        {
            if( (*it)->GetName() == name )
            {
                definition = *it;
                return true;
            }
        }

        return false;

    }

}