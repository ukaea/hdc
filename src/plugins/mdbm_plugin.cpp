#include <Pluma/Connector.hpp>
#include "plugins/mdbm_plugin.hpp"

PLUMA_CONNECTOR
bool connect(pluma::Host& host) {
    host.add( new MDBMStorageProvider() );
    return true;
}
