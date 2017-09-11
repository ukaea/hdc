#ifndef KVMDBM
#define KVMDBM
#include <string>
#include "storage_interface.hpp"
#include <mdbm.h>
#include <iostream>
#include <cstdio>
#include <json/json.h>
#include <sstream>
#include <boost/filesystem.hpp>
#include <hdc_helpers.h>

using namespace std;

class MDBMStorage : public Storage {
private:
    MDBM* db;
    bool initialized = false;
    bool persistent = true;
    string filename;
public:
    MDBMStorage() {
        DEBUG_STDOUT("MDBMStorage()\n");
    };
    ~MDBMStorage() {
        DEBUG_STDOUT("~MDBMStorage()\n");
        if(!persistent) cleanup();
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
        mdbm_close(this->db);
        // Remove db file if the data persistence is not required
        if (!this->persistent) {
            if (::remove(filename.c_str()) != 0) {
                perror("Error opening deleting file");
                exit(1);
            }
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
        if (boost::filesystem::exists(settings)) {
            stringstream ss(settings);
            ss >> root;
        }
        filename = root.get("filename","/tmp/db.mdbm").asString();
        persistent = root.get("persistent",true).asBool();
        D(printf("Filename: %s\n", filename.c_str());)
        this->db = mdbm_open(filename.c_str(), MDBM_O_RDWR | MDBM_O_CREAT | MDBM_LARGE_OBJECTS, 0666, 0, 0);
//         mdbm_set_alignment(this->db,MDBM_ALIGN_16_BITS);
        initialized = true;
        return;
    }
    void init(Json::Value& root) {
        filename = root.get("filename","/tmp/db.mdbm").asString();
        persistent = root.get("persistent",true).asBool();
        D(printf("Filename: %s\n", filename.c_str());)
        this->db = mdbm_open(filename.c_str(), MDBM_O_RDWR | MDBM_O_CREAT | MDBM_LARGE_OBJECTS, 0666, 0, 0);
//         mdbm_set_alignment(this->db,MDBM_ALIGN_16_BITS);
        initialized = true;
        return;
    }
};

PLUMA_INHERIT_PROVIDER(MDBMStorage, Storage);

#endif //KVMDBM
