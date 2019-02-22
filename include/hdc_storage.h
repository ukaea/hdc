#ifndef HDC_STORAGE_H
#define HDC_STORAGE_H

#include <Pluma/Pluma.hpp>
#include "plugins/storage_interface.hpp"
#include "plugins/umap_plugin.hpp" // Add this as fallback
#include <iostream>
#include <cstdio>
#include <hdc_helpers.h>

using namespace std;

class HDCStorage {
private:
    pluma::Pluma _pluma;
    Storage* _store;
    string pluginPath;
    size_t _id;
    bool _do_init;
public:
    HDCStorage(size_t _id, std::string name, std::string settings_str, bool do_init=true) {
        _pluma.acceptProviderType<StorageProvider>();
        //_pluma.addProvider( new UnorderedMapStorageProvider() ); // Add Unordered map storage as fallback
        if (name.size() != 0) {
            if (!_pluma.load(name)) {
                DEBUG_STDERR("Could not load plugin \"" +name +"\"\n");
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
        if (_do_init) _store->init(settings_str);
        this->pluginPath = name;
        this->_id = _id;
        DEBUG_STDOUT(_store->getDescription());
    }
    ~HDCStorage() {
        if (_do_init) _store->cleanup();
        delete _store;
        _pluma.unloadAll();
    }
    bool usesBuffersDirectly() {
        return _store->usesBuffersDirectly();
    }
    string getDescription() {
        return _store->getDescription();
    }
    std::string get_settings() {
        return _store->get_settings();
    }
    void set(string path, char* data, size_t size) {
        _store->set(path, data, size);
    }
    char* get(string path) {
        return _store->get(path);
    }
    void cleanup() {
        _store->cleanup();
    }
    size_t get_size(string path) {
        return _store->get_size(path);
    }
    bool has(string path) {
        return _store->has(path);
    }
    void remove(string path) {
        _store->remove(path);
    }
    void lock(string path) {
        _store->lock(path);
    };
    void unlock(string path) {
        _store->unlock(path);
    };
    bool locked() {
        return _store->locked();
    };
    void sync() {
        _store-> sync();
    };
    string name() {
        return _store->name();
    }
    size_t id() {
        return _id;
    }
};

#endif
