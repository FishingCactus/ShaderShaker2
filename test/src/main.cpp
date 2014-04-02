#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main( int argc, char* const argv[] )
{
  // global setup...

  int result = Catch::Session().run( argc, argv );

  // global clean-up...

  return result;
}