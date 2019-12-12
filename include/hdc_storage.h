#ifndef HDC_STORAGE_H
#define HDC_STORAGE_H

#include <Pluma/Pluma.hpp>
#include "plugins/storage_interface.hpp"
#include "plugins/umap_plugin.hpp" // Add this as fallback
#include <iostream>
#include <cstdio>
#include <hdc_helpers.h>
#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>

class HDCStorage {
private:
    pluma::Pluma _pluma;
    Storage* _store;
    std::string pluginPath;
    size_t _id;
    bool _do_init;

public:
    HDCStorage(size_t _id, const std::string& plugin_path, const std::string& settings_str, bool do_init=true) {
        _pluma.acceptProviderType<StorageProvider>();
        if (!plugin_path.empty()) {
            if (!_pluma.load(plugin_path)) {
                DEBUG_STDERR("Could not load plugin \"" + plugin_path +"\"\n");
                DEBUG_STDERR("Using std::unordered_map as fallback\n");
                _pluma.addProvider( new UnorderedMapStorageProvider() );
            }
        } else {
            _pluma.addProvider( new UnorderedMapStorageProvider() );
        }
        std::vector<StorageProvider*> providers;
        _pluma.getProviders(providers);
        _store = providers.front()->create();
        _do_init = do_init;
        if (_do_init) {
            _store->init(settings_str);
        }
        this->pluginPath = plugin_path;
        this->_id = _id;
        DEBUG_STDOUT(_store->getDescription());
    }

    ~HDCStorage() {
        if (_do_init) _store->cleanup();
        delete _store;
        _pluma.unloadAll();
    }

    std::string getDescription() {
        return _store->getDescription();
    }

    std::string get_settings() {
        return _store->get_settings();
    }

    void set(boost::uuids::uuid uuid, char* data, size_t size) {
        _store->set(uuid, data, size);
    }

    char* get(boost::uuids::uuid uuid) {
        return _store->get(uuid);
    }

    void cleanup() {
        _store->cleanup();
    }

    size_t get_size(boost::uuids::uuid uuid) {
        return _store->get_size(uuid);
    }

    bool has(boost::uuids::uuid uuid) {
        return _store->has(uuid);
    }

    void remove(boost::uuids::uuid uuid) {
        _store->remove(uuid);
    }

    void lock(boost::uuids::uuid uuid) {
        _store->lock(uuid);
    }

    void unlock(boost::uuids::uuid uuid) {
        _store->unlock(uuid);
    }

    bool locked() {
        return _store->locked();
    }

    void sync() {
        _store-> sync();
    }

    std::string name() {
        return _store->name();
    }

    size_t id() {
        return _id;
    }
};

#endif
