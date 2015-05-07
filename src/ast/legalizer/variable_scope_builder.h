#pragma once

#include "ast/const_tree_traverser.h"
#include "base/object_ref.h"
#include "base/object.h"
#include <string>
#include <vector>
#include <stack>
#include <map>

namespace AST
{
    namespace ScopeBuilder
    {
        struct Scope;

        struct Variable : public Base::Object
        {
            Variable(Scope & scope) : m_Scope(scope) {}

            std::string m_Type;
            std::string m_Name;
            const Scope & m_Scope;

        private:

            Variable& operator =( const Variable & );
        };

        struct Scope : public Base::Object
        {
            Scope() { }
            Scope( Scope & parent ) : m_Parent( &parent )
            {
                m_Parent->m_Children.push_back( this );
            }

            Scope * m_Parent; 
            std::string m_Name, m_Type;
            std::vector< Base::ObjectRef< Variable > > m_Variables;
            std::vector< Base::ObjectRef< Scope > > m_Children;

        private:

            Variable& operator =( const Variable & );
        };

        struct ScopeData
        {
            Scope m_GlobalScope;
            std::map< AST::Node *, Scope * > m_ScopeMap;
        };
    }

    class VariableScopeBuilder : public ConstTreeTraverser
    {
    public:

        VariableScopeBuilder( ScopeBuilder::ScopeData & scope_builder_data );

        virtual void Visit( const Node & node ) override;
        virtual void Visit( const TranslationUnit & node ) override;
        virtual void Visit( const VariableDeclaration & node ) override;
        virtual void Visit( const VariableDeclarationStatement & node ) override;
        virtual void Visit( const VariableDeclarationBody & node ) override;
        virtual void Visit( const FunctionDeclaration & node ) override;
        virtual void Visit( const Argument & node ) override;
        virtual void Visit( const StructDefinition & node ) override;

    private:

        struct NewScopeHelper
        {
            NewScopeHelper( VariableScopeBuilder & builder, const Node & node, const std::string & scope_name, const std::string & scope_type )
                : m_Builder( builder )
            {
                ScopeBuilder::Scope
                    * scope = new ScopeBuilder::Scope( m_Builder.GetCurrentScope() );

                scope->m_Name = scope_name;
                scope->m_Type = scope_type;

                m_Builder.m_ScopeData.m_ScopeMap.insert( std::make_pair( const_cast< Node * >( &node ), scope ) );
                m_Builder.m_ScopeStack.push( scope );
            }

            ~NewScopeHelper()
            {
                m_Builder.m_ScopeStack.pop();
            }

        private:

            NewScopeHelper& operator =( const NewScopeHelper & );

            VariableScopeBuilder & m_Builder;
        };

        VariableScopeBuilder& operator =( const VariableScopeBuilder & );

        ScopeBuilder::Variable * AddVariableToCurrentScope();
        ScopeBuilder::Scope & GetCurrentScope();

        ScopeBuilder::ScopeData & m_ScopeData;
        std::stack< ScopeBuilder::Scope * > m_ScopeStack;
        std::string m_CurrentVariableDeclarationType;
    };
}
