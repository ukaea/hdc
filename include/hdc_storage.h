#ifndef HDC_STORAGE_H
#define HDC_STORAGE_H

#include <Pluma/Pluma.hpp>
#include "plugins/storage_interface.hpp"
#include "plugins/umap_plugin.hpp" // Add this as fallback
#include <iostream>
#include <cstdio>
#include <hdc_helpers.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;

class HDCStorage {
private:
    pluma::Pluma _pluma;
    Storage* _store;
    boost::property_tree::ptree settings;
    string pluginPath;
public:
    HDCStorage(std::string name, std::string settings_str) {
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
        std::vector<StorageProvider*>::iterator it = providers.begin();
        if (!settings_str.empty()) {
            try {
                stringstream ss(settings_str);
                boost::property_tree::read_json(ss,settings);
            }
            catch (ifstream::failure e) {
                cout << "Error reading / opening file." << endl;
            }
        }
        _store = providers.front()->create();
        if (!settings.get("do_not_init",false))
            _store->init(settings);
        this->pluginPath = name;
        DEBUG_STDOUT(_store->getDescription());
    }

    HDCStorage(std::string name, boost::property_tree::ptree _settings) {
        _pluma.acceptProviderType<StorageProvider>();
        //_pluma.addProvider( new UnorderedMapStorageProvider() ); // Add Unordered map storage as fallback
        if (!name.empty()) {
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
        std::vector<StorageProvider*>::iterator it = providers.begin();
        _store = providers.front()->create();
        this->settings = _settings;
        this->pluginPath = name;
        if (!settings.get("do_not_init",false))
            _store->init(this->settings);
        DEBUG_STDOUT(_store->getDescription());
    }
    ~HDCStorage() {
        if (!settings.get("do_not_init",false))
            _store->cleanup();
        delete _store;
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
    boost::property_tree::ptree get_status() {
        boost::property_tree::ptree root;
        root.put("storage", this->name());
        root.add_child("settings",this->settings);
        return root;
    };
    string name() {
        return _store->name();
    }
};

#endif
