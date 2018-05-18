#ifndef UMAP_PLUGIN_HPP
#define UMAP_PLUGIN_HPP
#include <unordered_map>
#include <string>
#include <cstdio>
#include <boost/property_tree/ptree.hpp>
#include "storage_interface.hpp"
#include <hdc_helpers.h>
#include <exception>

// Stolen from MDBM:
struct datum {
    char* dptr;                 /**< Pointer to key or value data */
    size_t  dsize;                 /**< Number of bytes */
};

using namespace std;

class UnorderedMapStorage : public Storage {
private:
    unordered_map<string,datum> _map;
public:
    UnorderedMapStorage() {
        DEBUG_STDOUT("UnorderedMapStorage()\n");
    };
    ~UnorderedMapStorage() {
        DEBUG_STDOUT("~UnorderedMapStorage()\n");
    };
    bool usesBuffersDirectly() {
        return true;
    };
    void lock(string path) {};
    void unlock(string path) {};
    bool locked() {
        return false;
    };
    void sync() {};

    string getDescription() {
        return "This is unordered_map based storage.";
    };
    void set(string key, char* data, size_t size) {
        if (_map.find(key) != _map.end() && _map[key].dptr != data) {
            delete[] _map[key].dptr;
            _map.erase(key);
        }
        _map[key] = {data,size};
        return;
    };
    char* get(string key) {
        if (_map.find(key) == _map.end()) {
            throw std::runtime_error("UnorderedMapStorage::get("+key+"): not found\n");
        }
        return (char*)_map[key].dptr;
    };
    size_t get_size(string key) {
        if (_map.find(key) == _map.end()) {
            throw std::runtime_error("UnorderedMapStorage::get("+key+"): not found\n");
        }
        return _map[key].dsize;
    };
    bool has(string key) {
        return (_map.find(key) != _map.end());
    };
    void cleanup () {
        DEBUG_STDOUT("UnorderedMapStorage::cleanup()\n");
        return;
    };
    void remove(string key) {
        if (has(key)) {
            delete[] _map[key].dptr;
            _map.erase(key);
        }
        return;
    };
    void init(string settings) {
        // Nothing to set here ...
        DEBUG_STDOUT("UnorderedMapStorage::init()\n");
    };
    void init(boost::property_tree::ptree& settings) {
        // Nothing to set here ...
        DEBUG_STDOUT("UnorderedMapStorage::init()\n");
    };
    string name() {
        return "umap";
    }
};

PLUMA_INHERIT_PROVIDER(UnorderedMapStorage, Storage);

#endif //UMAP_PLUGIN_HPP

