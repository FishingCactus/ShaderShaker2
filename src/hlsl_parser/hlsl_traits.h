#ifndef HLSL_TRAITS_H
    #define HLSL_TRAITS_H

    #include <antlr3.hpp>

    class HLSLLexer; class HLSLParser;

    template<class ImplTraits>
    class HLSLUserTraits : public antlr3::CustomTraitsBase<ImplTraits> {};

    class HLSLLexerTraits : public antlr3::Traits< HLSLLexer, HLSLParser, HLSLUserTraits >
    {
        public:

        static int ConvertToInt32( const std::string & type )
        {
            int
                return_value;

            std::istringstream( type ) >> return_value;

            return return_value;
        }
    };

    typedef HLSLLexerTraits HLSLParserTraits;

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