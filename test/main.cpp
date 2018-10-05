//#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <hdc.hpp>
int main( int argc, char* argv[] ) {
    int result = -1;
    Catch::Session session;
    HDC::search_plugins();
    auto plugins = HDC::get_available_plugins();
    for (auto plugin : plugins) {
        std::cout << "--- Testing \"" << plugin << "\" plugin..." << std::endl;
        HDC::init(plugin, "{\"persistent\": false}");
        result = session.run( argc, argv );
        HDC::destroy();
        if (result) break;
    }
  return result;
}
