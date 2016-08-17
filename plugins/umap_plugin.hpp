#ifndef KVSTDMAP
#define KVSTDMAP
#include <unordered_map>
#include <string>
#include <iostream>
#include "storage_interface.hpp"


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
        cout << "UnorderedMapStorage()" << endl;
    };
    ~UnorderedMapStorage() {
        cout << "~UnorderedMapStorage()" << endl;
    };

    string getDescription() {
        return "This is unordered_map based storage.";
    };
    void set(string key, char* data, size_t size) {
        if (_map.find(key) != _map.end()) {
            delete[] _map[key].dptr;
            _map.erase(key);
        }
        _map[key] = {data,size};
        return;
    };
    char* get(string key) {
        if (_map.find(key) == _map.end()) {
            cout << "get(): not found" << endl;
            exit(-3);
        }
        return (char*)_map[key].dptr;
    };
    size_t get_size(string key) {
        if (_map.find(key) == _map.end()) {
            cout << "get_size(): not found" << endl;
            exit(-3);
        }
        return _map[key].dsize;
    };
    bool has(string key) {
        return (_map.find(key) != _map.end());
    };
    void cleanup () {
        cout << "UnorderedMapStorage::cleanup()" << endl;
        return;
    };
    void remove(string key) {
        _map.erase(key);
        return;
    };
    void init(string settings) {
        // Nothing to set here ...
        cout << "Initialized." << endl;
    };
};

PLUMA_INHERIT_PROVIDER(UnorderedMapStorage, Storage);

#endif //KVSTDMAP

