#ifndef GLSL_PRINTER
    #define GLSL_PRINTER

    #include "ast/printer/xlsl_printer.h"
    #include <ostream>
    #include <map>

    namespace AST
    {

        class GLSLPrinter : public XLSLPrinter
        {
        public:

            GLSLPrinter( std::ostream & stream );

            using XLSLPrinter::Visit;
            virtual void Visit( const VariableDeclarationBody & body ) override;
            virtual void Visit( const IntrinsicType & type ) override;
            virtual void Visit( const InitialValue & initial_value ) override;
            virtual void Visit( const Annotations & annotations ) override;
            virtual void Visit( const AnnotationEntry & annotation_entry ) override;
            virtual void Visit( const TextureDeclaration & declaration ) override;
            virtual void Visit( const SamplerDeclaration & declaration ) override;
            virtual void Visit( const SamplerBody & body ) override;
            virtual void Visit( const StructDefinition & definition ) override;
            virtual void Visit( const FunctionDeclaration & declaration ) override;
            virtual void Visit( const Argument & argument ) override;
            virtual void Visit( const DiscardStatement & statement ) override;

        private:

            void InitializeTypeLookupTable();

            typedef std::map< std::string, std::string >
                TypeLookupMap;

            TypeLookupMap
                m_TypeLookupMap;
        };
    }
#endif
