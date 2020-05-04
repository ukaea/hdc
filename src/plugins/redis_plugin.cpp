#include <Pluma/Connector.hpp>
#include "plugins/redis_plugin.hpp"

PLUMA_CONNECTOR
bool connect(pluma::Host& host) {
    host.add( new RedisStorageProvider() );
    return true;
}
