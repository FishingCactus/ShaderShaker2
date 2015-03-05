#ifndef HLSL_TRAITS_H
    #define HLSL_TRAITS_H

    #include <antlr3.hpp>
    #include "../ast/node.h"

    class HLSLLexer; class HLSLParser;

    template<class ImplTraits>
    class HLSLUserTraits : public antlr3::CustomTraitsBase<ImplTraits> {};

    struct HLSLLexerTraits : public antlr3::Traits< HLSLLexer, HLSLParser, HLSLUserTraits >
    {
        static int ConvertToInt32( const std::string & type )
        {
            int
                return_value;

            std::istringstream( type ) >> return_value;

            return return_value;
        }
    };

    struct HLSLParserTraits : public HLSLLexerTraits
    {
        class RuleReturnValueType : public antlr3::RuleReturnValue<HLSLParserTraits>
        {
            const CommonTokenType*  m_StartToken;
            int m_PreviousLine;
            StringType m_PreviousFile;

        public:

            RuleReturnValueType()
                : RuleReturnValue(),
                m_StartToken( 0 ),
                m_PreviousLine( -1 ),
                m_PreviousFile()
            {
            }

            RuleReturnValueType( BaseParserType* parser )
                : RuleReturnValue( parser ),
                m_StartToken( parser->LT( 1 ) ),
                m_PreviousLine( AST::Node::GetCurrentLine() ),
                m_PreviousFile( AST::Node::GetCurrentFileName() )
            {
                AST::Node::SetDebugInfo(
                    m_StartToken->get_input()->get_fileName(),
                    m_StartToken->get_line()
                    );
            }

            RuleReturnValueType( const RuleReturnValueType& other )
                : RuleReturnValue( other ),
                m_StartToken( other.m_StartToken ),
                m_PreviousLine( other.m_PreviousLine ),
                m_PreviousFile( other.m_PreviousFile )
            {
            }

            RuleReturnValueType& operator=( const RuleReturnValueType& other )
            {
                RuleReturnValue::operator=( other );
                m_StartToken = other.m_StartToken;
                m_PreviousLine = other.m_PreviousLine;
                m_PreviousFile = other.m_PreviousFile;

                return *this;
            }

            ~RuleReturnValueType()
            {
                AST::Node::SetDebugInfo( m_PreviousFile, m_PreviousLine );
            }
        };
    };

    ANTLR_BEGIN_NAMESPACE()

    template<>
    class FileUtils< TraitsBase< HLSLUserTraits > > : public FileUtils< TraitsBase< CustomTraitsBase > >
    {
    public:
        typedef TraitsBase< HLSLUserTraits > ImplTraits;
        typedef void ( *Callback )( void * content, size_t & size, const char * );

        template<typename InputStreamType>
        static ANTLR_UINT32 AntlrRead8Bit(InputStreamType* input, const ANTLR_UINT8* fileName)
        {
            if ( ReadFileContentCallback )
            {
                size_t
                    length;
                void
                    * content;

                ReadFileContentCallback( NULL, length, ( const char * ) fileName );

                if ( !length )
                {
                    return ANTLR_FAIL;
                }

                content = InputStreamType::AllocPolicyType::alloc( length );
                ReadFileContentCallback( content, length, ( const char * ) fileName );

                input->set_data( (unsigned char*) content );
                input->set_sizeBuf( length );

                input->set_isAllocated( true );

                return ANTLR_SUCCESS;
            }
            else
            {
                return FileUtils< TraitsBase< CustomTraitsBase > >::AntlrRead8Bit( input, fileName );
            }
        }

        static Callback ReadFileContentCallback;
    };

    ANTLR_END_NAMESPACE()

#endif