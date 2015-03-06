#ifndef HLSL_PRINTER
    #define HLSL_PRINTER

    #include "ast/printer/xlsl_printer.h"
    #include <ostream>

    namespace AST
    {

        class HLSLPrinter : public XLSLPrinter
        {
        public:

            HLSLPrinter( std::ostream & stream ) : XLSLPrinter( stream ){}

            using XLSLPrinter::Visit;
            virtual void Visit( const VariableDeclarationBody & body ) override;
            virtual void Visit( const Type & type ) override;
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
        };
    }
#endif
