//#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <hdc.hpp>
int main( int argc, char* argv[] ) {
    int result = -1;
    Catch::Session session;
    auto plugins = HDC::search_plugins();
    for (auto plugin : plugins) {
        std::cout << "--- Testing \"" << plugin.first << "\" plugin found in " << plugin.second << " ..." << std::endl;
        HDC::init(plugin.first, "{\"persistent\": false}");
        result = session.run( argc, argv );
        HDC::destroy();
        if (result) break;
    }
  return result;
}
