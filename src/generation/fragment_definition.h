#ifndef FRAGMENT_DEFINITION_H
    #define FRAGMENT_DEFINITION_H

	#include <memory>
    #include <vector>
    #include "base/object.h"
    #include "base/object_ref.h"

	namespace AST
	{
		struct TranslationUnit;
	}

	namespace Generation
	{
		class FunctionDefinition;

		class FragmentDefinition : public Base::Object
		{

		public:

			static void GenerateFragment(
				Base::ObjectRef<FragmentDefinition> & definition,
				AST::TranslationUnit & translation_unit
				);


            bool FindFunctionDefinition(
                Base::ObjectRef<FunctionDefinition> & definition,
                const std::string & name
                );

		private:

			Base::ObjectRef<AST::TranslationUnit>
				m_AbstractSyntaxTree;
			std::vector<Base::ObjectRef<FunctionDefinition> >
				m_FunctionDefinitionTable;

		};

	}

#endif