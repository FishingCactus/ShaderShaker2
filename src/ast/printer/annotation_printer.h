#ifndef ANNOTATION_PRINTER
    #define ANNOTATION_PRINTER

    #include "ast/tree_traverser.h"
    #include <ostream>

    namespace AST
    {
        class AnnotationPrinter : public TreeTraverser
        {
        public:

            AnnotationPrinter( std::ostream & stream ) : m_Stream( stream ), m_InsideInitialValue(false){}

            virtual void Visit( const Node & node ) override;
            virtual void Visit( const Annotations & annotations ) override;
            virtual void Visit( const AnnotationEntry & annotation_entry ) override;
            virtual void Visit( const VariableDeclarationBody & body ) override;
            virtual void Visit( const InitialValue & initial_value ) override;
            virtual void Visit( const ConstructorExpression & expression ) override;
            virtual void Visit( const ArgumentExpressionList & list ) override;
            virtual void Visit( const LiteralExpression & expression ) override;
            virtual void Visit( const TextureDeclaration & body ) override;
            virtual void Visit( const TranslationUnit & translation_unit ) override;

        private:

            AnnotationPrinter & operator =( const AnnotationPrinter & );

            std::ostream
                & m_Stream;
            bool
                m_InsideInitialValue;
        };
    }
#endif
