#ifndef HDC_STORAGE_H
#define HDC_STORAGE_H

#include <Pluma/Pluma.hpp>
#include "storage_interface.hpp"
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
        /*if (!_pluma.load("plugins",name)) {
            cout << "Could not load plugin \"" << name << "\"" << endl;
            return;
        }*/
        if (!_pluma.load("/home/david/projects/hdc_new/build/bin/plugins","libUMapPlugin.so")) {
            cout << "Could not load plugin \"" << name << "\"" << endl;
            return;
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
};

#endif