#ifndef HDC_STORAGE_H
#define HDC_STORAGE_H

#include <Pluma/Pluma.hpp>
#include "storage_interface.hpp"
#include "../plugins/umap_plugin.hpp" // Add this as fallback
#include <iostream>
#include <cstdio>

using namespace std;

class HDCStorage {
private:
    pluma::Pluma _pluma;
    Storage* _store;
public:
    HDCStorage(std::string name, std::string settings) {
        _pluma.acceptProviderType<StorageProvider>();
        //_pluma.addProvider( new UnorderedMapStorageProvider() ); // Add Unordered map storage as fallback
        if (name.size() != 0) {
            if (!_pluma.load(name)) {
                cerr << "Could not load plugin \"" << name << "\"" << endl;
                cerr << "Using std::unordered_map as fallback" << endl;
                _pluma.addProvider( new UnorderedMapStorageProvider() );
            }
        } else {
            _pluma.addProvider( new UnorderedMapStorageProvider() );
        }
        std::vector<StorageProvider*> providers;
        _pluma.getProviders(providers);
        std::vector<StorageProvider*>::iterator it = providers.begin();
        _store = providers.front()->create();
        _store->init(settings);
        std::cout << _store->getDescription() << std::endl;
    }
    ~HDCStorage() {
        _store->cleanup();
        //delete _store;
        _pluma.unloadAll();
    }
    bool usesBuffersDirectly() {
        return _store->usesBuffersDirectly();
    }
    string getDescription() {
        return _store->getDescription();
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
};

#endif
