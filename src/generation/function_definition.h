#ifndef FUNCTION_DEFINITION_H
    #define FUNCTION_DEFINITION_H

    #include <set>
    #include <string>
    #include <memory>
    #include "fragment_definition.h"
    #include "base/object.h"
    #include "base/object_ref.h"
    namespace AST{ struct FunctionDeclaration; }

    namespace Generation
    {
        class FunctionDefinition : public Base::Object
        {

        public:

            typedef Base::ObjectRef<FunctionDefinition>
                Ref;

            void FillFromFunctionDeclaration(
                AST::FunctionDeclaration & declaration
                );

            const std::set<std::string> & GetInSemanticSet() const
            {
                return m_InSemanticSet;
            }

            const std::set<std::string> & GetOutSemanticSet() const
            {
                return m_OutSemanticSet;
            }

            const std::set<std::string> & GetInOutSemanticSet() const
            {
                return m_InOutSemanticSet;
            }

            const std::string & GetName() const { return m_Name; }

        private:

            std::string
                m_Name;
            std::set<std::string>
                m_InSemanticSet,
                m_OutSemanticSet,
                m_InOutSemanticSet;

        };

    }
#endif