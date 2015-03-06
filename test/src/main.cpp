#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main( int argc, char* const argv[] )
{
    Catch::Session
        session;
    int
        result;

    result = session.applyCommandLine( argc, argv );

    if ( result == 0 )
    {
        if ( Catch::isDebuggerActive() )
            session.config().useStream( "debug" );

        result = session.run();
    }

    return result;
}