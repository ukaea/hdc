#ifndef KVMDBM
#define KVMDBM
#include <string>
#include "storage_interface.hpp"
#include <mdbm.h>
#include <iostream>
#include <json/json.h>
#include <sstream>

using namespace std;

class MDBMStorage : public Storage {
private:
    MDBM* db;
    bool initialized = false;
public:
    MDBMStorage() {
        cout << "MDBMStorage()" << endl;
    };
    ~MDBMStorage() {
        cout << "~MDBMStorage()" << endl;
        cleanup();
    };
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
        cout << "MDBMStorage::cleanup()" << endl;
        mdbm_close(this->db);
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
        stringstream ss(settings);
        ss >> root;
        string filename = root.get("filename","/tmp/db.mdbm").asString();
        printf("Filename: %s\n", filename.c_str());
        this->db = mdbm_open(filename.c_str(), MDBM_O_RDWR | MDBM_O_CREAT | MDBM_LARGE_OBJECTS, 0666, 0, 0);
        initialized = true;
        return;
    }
};

PLUMA_INHERIT_PROVIDER(MDBMStorage, Storage);

#endif //KVMDBM
