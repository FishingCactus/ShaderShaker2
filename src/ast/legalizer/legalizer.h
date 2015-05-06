#pragma once

#include "ast/tree_traverser.h"
#include "base/object_ref.h"

namespace AST
{
    class Legalizer : public TreeTraverser
    {
    public:

        virtual void Visit( Node & node ) override;
        virtual void Visit( VariableDeclaration & node ) override;
        virtual void Visit( LiteralExpression & node ) override;

    private:

        Legalizer& operator =( const Legalizer & );

        VariableDeclaration * m_CurrentVariableDeclaration;
    };
}
