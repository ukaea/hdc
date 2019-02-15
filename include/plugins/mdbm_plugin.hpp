#ifndef MDBM_PLUGIN_HPP
#define MDBM_PLUGIN_HPP
#include <string>
#include "storage_interface.hpp"
#include <mdbm.h>
#include <iostream>
#include <cstdio>
#include <sstream>
#include <hdc_helpers.h>
#include <exception>
#include <sys/stat.h>
#include <json/json.h>

using namespace std;

/**
 * Check if a file exists
 * @return true if and only if the file exists, false else
 */
bool MDBMFileExists(const std::string& file) {
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
}

class MDBMStorage : public Storage {
private:
    MDBM* db = NULL;
    bool initialized = false;
    StorageOptions options;
public:
    MDBMStorage() {
        DEBUG_STDOUT("MDBMStorage()\n");
    };
    ~MDBMStorage() {
        DEBUG_STDOUT("~MDBMStorage()\n");
        if(!options.persistent) {
            cleanup();
        } else std::cout << "Storage has been set persistent. The File database has been stored in file \"" << options.filename << "\""<< std::endl;
    };
    bool usesBuffersDirectly() {
        return false;
    };
    void lock(string path) {
        datum key = {&path[0u], static_cast<int>(path.length())};
        mdbm_lock_smart (this->db, &key, 0);
    };
    void unlock(string path) {
        datum key = {&path[0u], static_cast<int>(path.length())};
        mdbm_unlock_smart (this->db, &key, 0);
    };
    bool locked() {
        return mdbm_islocked(this->db);
    };
    void sync() {};
    string getDescription() {
        return "This is MDBM based storage.";
    };
    void set(string path, char* data, size_t size) {
        datum key = {&path[0u], static_cast<int>(path.length())};
        datum value = {(char*)data, static_cast<int>(size)};
        mdbm_lock_smart (this->db, &key, 0);
        mdbm_store(this->db,key,value,MDBM_REPLACE);
        mdbm_unlock_smart (this->db, &key, 0);
        return;
    };
    char* get(string path) {
        datum key = {&path[0u], static_cast<int>(path.length())};
        datum found = mdbm_fetch(this->db, key);
        return (char*) found.dptr;
    };
    size_t get_size(string path) {
        datum key = {&path[0u], static_cast<int>(path.length())};
        datum found = mdbm_fetch(this->db, key);
        return found.dsize;
    };
    void cleanup () {
        DEBUG_STDOUT("MDBMStorage::cleanup()\n");
        if (this->db != NULL) {
            mdbm_close(this->db);
            mdbm_delete_lockfiles(this->options.filename.c_str());
            this->db = NULL;
        }
        // Remove db file if the data persistence is not required
        if (!this->options.persistent && MDBMFileExists(options.filename)) {
            if (::remove(options.filename.c_str()) != 0) {
                throw std::runtime_error("MDBMStorage::cleanup(): Error deleting file\n");
            };
        }
        initialized = false;
        return;
    };
    bool has(string path) {
        datum key = {&path[0u], static_cast<int>(path.length())};
        return (mdbm_fetch(this->db, key).dsize != 0);
    };
    void remove(string path) {
        datum key = {&path[0u], static_cast<int>(path.length())};
        mdbm_delete(this->db,key);
        return;
    };
    void init(string settings) {
        Json::Value root;
        std::stringstream ss;
        ss << settings;
        ss >> root;
        StorageOptions so;
        so.filename = root.get("filename","/tmp/db.mdbm").asString();
        so.persistent = root.get("persistent",false).asBool();
        init(so);
        return;
    }
    void init(StorageOptions& _options) {
        options = _options;
        D(printf("Filename: %s\n", filename.c_str());)
        this->db = mdbm_open(options.filename.c_str(), MDBM_O_RDWR | MDBM_O_CREAT | MDBM_LARGE_OBJECTS, 0666, 0, 0);
        mdbm_set_alignment(this->db,MDBM_ALIGN_16_BITS);
        initialized = true;
        return;
    }
    string name() {
        return "mdbm";
    }
};

PLUMA_INHERIT_PROVIDER(MDBMStorage, Storage);

#endif //MDBM_PLUGIN_HPP
