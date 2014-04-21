#ifndef HLSL_PRINTER
    #define HLSL_PRINTER

    #include "ast/visitor.h"
    #include <ostream>

    namespace AST
    {

        class HLSLPrinter : public Visitor
        {
        public:

            HLSLPrinter( std::ostream & stream ) : m_Stream( stream ){}

            virtual void Visit( Node & node ) override;
            virtual void Visit( TranslationUnit & translation_unit ) override;
            virtual void Visit( VariableDeclaration & variable_declaration ) override;
            virtual void Visit( IntrinsicType & type ) override;
            virtual void Visit( UserDefinedType & type ) override;
            virtual void Visit( SamplerType & type ) override;
            virtual void Visit( TypeModifier & modifier ) override;
            virtual void Visit( StorageClass & storage_class ) override;
            virtual void Visit( VariableDeclarationBody & body ) override;
            virtual void Visit( TextureDeclaration & declaration ) override;
            virtual void Visit( SamplerDeclaration & declaration ) override;
            virtual void Visit( SamplerBody & body ) override;
            virtual void Visit( StructDefinition & definition ) override;
            virtual void Visit( LiteralExpression & expression ) override;
            virtual void Visit( VariableExpression & expression ) override;
            virtual void Visit( UnaryOperationExpression & expression ) override;
            virtual void Visit( CallExpression & expression ) override;
            virtual void Visit( ArgumentExpressionList & list ) override;

        private:

            std::ostream
                & m_Stream;

        };
    }


#endif
