#ifndef STRING_HELPER_H
    #define STRING_HELPER_H

    #include <algorithm>
    #include <string>

    void tolower_in_place( std::string & string_to_convert )
    {
        std::transform(string_to_convert.begin(),string_to_convert.end(),string_to_convert.begin(), [](unsigned char c)
        {
            return std::tolower(c);
        });
    }

#endif
