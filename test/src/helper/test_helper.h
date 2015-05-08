#pragma once

#include <string>
#include <algorithm>

namespace TestHelper
{
    namespace
    {
        bool BothAreSpaces( char lhs, char rhs )
        {
            return ( lhs == rhs ) && ( lhs == ' ' );
        }

        void RemoveDuplicateSpaces( std::string & str )
        {
            std::string::iterator new_end = std::unique( str.begin(), str.end(), BothAreSpaces );
            str.erase( new_end, str.end() );
        }

        void ReplaceNonPrintableCharacters( std::string & str )
        {
            std::replace( str.begin(), str.end(), '\n', ' ' );
            std::replace( str.begin(), str.end(), '\t', ' ' );
        }

        void Trim( std::string & str )
        {
            const auto first_char_ite = str.find_first_not_of( " \n\t" );
            
            if ( first_char_ite != std::string::npos )
            {
                const auto last_char_ite = str.find_last_not_of( " \n\t" );
                const auto range = last_char_ite - first_char_ite + 1;

                str.substr( first_char_ite, range );
            }
        }
    }

    std::string GetCleanedString( const std::string & str )
    {
        std::string result = str;

        ReplaceNonPrintableCharacters( result );
        Trim( result );
        RemoveDuplicateSpaces( result );

        return result;
    }
}