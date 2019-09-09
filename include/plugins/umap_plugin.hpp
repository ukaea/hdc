#ifndef UMAP_PLUGIN_HPP
#define UMAP_PLUGIN_HPP

#include <unordered_map>
#include <string>
#include <cstdio>
#include <boost/property_tree/ptree.hpp>
#include "storage_interface.hpp"
#include <hdc_helpers.h>
#include <exception>
#include <vector>

// Stolen from MDBM:
struct datum {
    char* dptr;                 /**< Pointer to key or value data */
    size_t dsize;               /**< Number of bytes */
};

class UnorderedMapStorage : public Storage {
private:
    std::unordered_map<std::string, std::vector<char> > _map;

public:
    UnorderedMapStorage() {
        DEBUG_STDOUT("UnorderedMapStorage()\n");
    }

    ~UnorderedMapStorage() override {
        DEBUG_STDOUT("~UnorderedMapStorage()\n");
    }

    void lock(std::string path UNUSED) override {}

    void unlock(std::string path UNUSED) override {}

    bool locked() override {
        return false;
    }

    void sync() override {}

    std::string getDescription() override {
        return "This is unordered_map based storage.";
    }

    std::string get_settings() override {
        return "{}";
    }

    void set(std::string key, char* data, size_t size) override {
        if (_map.find(key) != _map.end() && &(_map[key])[0] != data) {
            _map.erase(key);
        }
        //_map[key] = {data,size};
        _map.emplace(key, std::vector<char>(data,data+size));
    };

    char* get(std::string key) override {
        if (_map.find(key) == _map.end()) {
            throw std::runtime_error("UnorderedMapStorage::get("+key+"): not found\n");
        }
        return &(_map[key])[0];
    }

    size_t get_size(std::string key) override {
        if (_map.find(key) == _map.end()) {
            throw std::runtime_error("UnorderedMapStorage::get("+key+"): not found\n");
        }
        return _map[key].size();
    }

    bool has(std::string key) override {
        return (_map.find(key) != _map.end());
    }

    void cleanup () override {
        DEBUG_STDOUT("UnorderedMapStorage::cleanup()\n");
        _map.clear();
    }

    void remove(std::string key) override {
        if (has(key)) {
            _map.erase(key);
        }
    }

    void init(std::string settings UNUSED) override {
        DEBUG_STDOUT("UnorderedMapStorage::init()\n");
    }

    std::string name() override {
        return "umap";
    }
};

PLUMA_INHERIT_PROVIDER(UnorderedMapStorage, Storage)

#endif //UMAP_PLUGIN_HPP

