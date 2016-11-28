#include "semantic_remover.h"

#include <vector>
#include <base/object_ref.h>
#include <ast/node.h>

namespace Generation
{
	void SemanticRemover::Visit( AST::TranslationUnit & translation_unit )
	{
		std::vector< Base::ObjectRef<AST::GlobalDeclaration> >::iterator it, end;

		it = translation_unit.m_GlobalDeclarationTable.begin();
		end = translation_unit.m_GlobalDeclarationTable.end();

		for( ; it != end; ++it )
		{
			(*it)->Visit( *this );
		}
	}

	void SemanticRemover::Visit( AST::FunctionDeclaration & function_declaration )
	{
		function_declaration.m_Semantic.clear();

		if ( function_declaration.m_ArgumentList )
		{
			function_declaration.m_ArgumentList->Visit( *this );
		}
	}

	void SemanticRemover::Visit( AST::ArgumentList & argument_list )
	{
		std::vector< Base::ObjectRef<AST::Argument> >::iterator it, end;

		it = argument_list.m_ArgumentTable.begin();
		end = argument_list.m_ArgumentTable.end();

		for( ; it != end; ++it )
		{
			(*it)->Visit( *this );
		}
	}

	void SemanticRemover::Visit( AST::Argument & argument )
	{
		argument.m_Semantic.clear();
	}
}