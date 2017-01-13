#ifndef STRING_HELPER_H
    #define STRING_HELPER_H

    #include <cctype>
    #include <string>

    void tolower_in_place( std::string & string_to_convert )
    {
        for ( std::string::size_type i = 0; i < string_to_convert.length(); ++i)
        {
            string_to_convert[i] = tolower( string_to_convert[i] );
        }
    }

#endif
