#ifndef INDENTATION_H
    #define INDENTATION_H

    #include <ostream>

    std::ios_base& inc_ind(std::ios_base& stream);
    std::ios_base& dec_ind(std::ios_base& stream);
    int get_indent_index();

    template<class charT, class traits>
    std::basic_ostream<charT, traits>& endl_ind( std::basic_ostream<charT, traits>& stream )
    {
        int
            indent;

        indent = stream.iword( get_indent_index() );
        stream.put( stream.widen( '\n' ) );

        while ( indent )
        {
            stream.put( stream.widen( '\t' ) );
            indent--;
        }

        stream.flush();

        return stream;
    }

#endif