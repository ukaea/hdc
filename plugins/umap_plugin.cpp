#include <Pluma/Connector.hpp>
#include "umap_plugin.hpp"

PLUMA_CONNECTOR
bool connect(pluma::Host& host) {
    host.add( new UnorderedMapStorageProvider() );
    return true;
}
