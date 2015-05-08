#ifndef ANNOTATION_PRINTER
    #define ANNOTATION_PRINTER

    #include "ast/tree_traverser.h"
    #include <ostream>

    namespace AST
    {
        class AnnotationPrinter : public TreeTraverser
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
#endif