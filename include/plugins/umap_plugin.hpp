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
    size_t  dsize;                 /**< Number of bytes */
};

using namespace std;

class UnorderedMapStorage : public Storage {
private:
    unordered_map<string,vector<char> > _map;
public:
    UnorderedMapStorage() {
        DEBUG_STDOUT("UnorderedMapStorage()\n");
    };
    ~UnorderedMapStorage() {
        DEBUG_STDOUT("~UnorderedMapStorage()\n");
    };
    void lock(string path UNUSED) {};
    void unlock(string path UNUSED) {};
    bool locked() {
        return false;
    };
    void sync() {};

    string getDescription() {
        return "This is unordered_map based storage.";
    };
    std::string get_settings() {
        return "{}";
    }
    void set(string key, char* data, size_t size) {
        if (_map.find(key) != _map.end() && &(_map[key])[0] != data) {
            _map.erase(key);
        }
        //_map[key] = {data,size};
        _map.emplace(key,vector<char>(data,data+size));
        return;
    };
    char* get(string key) {
        if (_map.find(key) == _map.end()) {
            throw std::runtime_error("UnorderedMapStorage::get("+key+"): not found\n");
        }
        return &(_map[key])[0];
    };
    size_t get_size(string key) {
        if (_map.find(key) == _map.end()) {
            throw std::runtime_error("UnorderedMapStorage::get("+key+"): not found\n");
        }
        return _map[key].size();
    };
    bool has(string key) {
        return (_map.find(key) != _map.end());
    };
    void cleanup () {
        DEBUG_STDOUT("UnorderedMapStorage::cleanup()\n");
        _map.clear();
        return;
    };
    void remove(string key) {
        if (has(key)) {
            _map.erase(key);
        }
        return;
    };
    void init(std::string settings UNUSED) {
        DEBUG_STDOUT("UnorderedMapStorage::init()\n");
    };
    string name() {
        return "umap";
    }
};

PLUMA_INHERIT_PROVIDER(UnorderedMapStorage, Storage)

#endif //UMAP_PLUGIN_HPP

