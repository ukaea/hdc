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

namespace std
{
    template<>
    struct hash<boost::uuids::uuid>
    {
        size_t operator () (const boost::uuids::uuid& uid)
        {
            return boost::hash<boost::uuids::uuid>()(uid);
        }
    };
}

class UnorderedMapStorage : public Storage {
private:
    //std::unordered_map<std::string, std::vector<char> > _map;
    std::unordered_map<boost::uuids::uuid, std::vector<char>, boost::hash<boost::uuids::uuid>> _map;
public:
    UnorderedMapStorage() {
        DEBUG_STDOUT("UnorderedMapStorage()\n");
    }

    ~UnorderedMapStorage() override {
        DEBUG_STDOUT("~UnorderedMapStorage()\n");
    }

    void lock(boost::uuids::uuid uuid UNUSED) override {}

    void unlock(boost::uuids::uuid uuid UNUSED) override {}

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

    void set(boost::uuids::uuid uuid, char* data, size_t size) override {
        if (_map.find(uuid) != _map.end() && &(_map[uuid])[0] != data) {
            _map.erase(uuid);
        }
        //_map[uuid] = {data,size};
        _map.emplace(uuid, std::vector<char>(data,data+size));
    };

    char* get(boost::uuids::uuid uuid) override {
        if (_map.find(uuid) == _map.end()) {
            throw std::runtime_error("UnorderedMapStorage::get("+boost::lexical_cast<std::string>(uuid)+"): not found\n");
        }
        return &(_map[uuid])[0];
    }

    size_t get_size(boost::uuids::uuid uuid) override {
        if (_map.find(uuid) == _map.end()) {
            throw std::runtime_error("UnorderedMapStorage::get("+boost::lexical_cast<std::string>(uuid)+"): not found\n");
        }
        return _map[uuid].size();
    }

    bool has(boost::uuids::uuid uuid) override {
        return (_map.find(uuid) != _map.end());
    }

    void cleanup () override {
        DEBUG_STDOUT("UnorderedMapStorage::cleanup()\n");
        _map.clear();
    }

    void remove(boost::uuids::uuid uuid) override {
        if (has(uuid)) {
            _map.erase(uuid);
        }
    }

    void init(std::string settings UNUSED) override {
        DEBUG_STDOUT("UnorderedMapStorage::init()\n");
    }

    std::string name() override {
        return "umap";
    }

    bool memory_mapped() override {
        return true;
    }
};

PLUMA_INHERIT_PROVIDER(UnorderedMapStorage, Storage)

#endif //UMAP_PLUGIN_HPP

