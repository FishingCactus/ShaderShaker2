#pragma once

#include "ast/const_tree_traverser.h"
#include <iosfwd>

namespace AST
{
    class AnnotationPrinter : public ConstTreeTraverser
    {
    public:

        AnnotationPrinter( std::ostream & stream ) : m_Stream( stream ){}

        virtual void Visit( const Node & node ) override;
        virtual void Visit( const Annotations & annotations ) override;
        virtual void Visit( const AnnotationEntry & annotation_entry ) override;

    private:

        AnnotationPrinter & operator =( const AnnotationPrinter & );

        std::ostream
            & m_Stream;
    };
}