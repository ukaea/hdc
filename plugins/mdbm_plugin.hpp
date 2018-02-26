#ifndef MDBM_PLUGIN_HPP
#define MDBM_PLUGIN_HPP
#include <string>
#include "storage_interface.hpp"
#include <mdbm.h>
#include <iostream>
#include <cstdio>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <boost/filesystem.hpp>
#include <hdc_helpers.h>
#include "hdc_errors.hpp"

using namespace std;

class MDBMStorage : public Storage {
private:
    MDBM* db;
    bool initialized = false;
    bool persistent = false;
    string filename;
public:
    MDBMStorage() {
        DEBUG_STDOUT("MDBMStorage()\n");
    };
    ~MDBMStorage() {
        DEBUG_STDOUT("~MDBMStorage()\n");
        cout << "in destructor\n";
        if(!persistent) {
            cout << "Calling cleanup()\n";
            cleanup();
        } else cout << "persistent!!!\n";
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
        mdbm_delete_lockfiles(this->filename.c_str());
        // Remove db file if the data persistence is not required
        if (!this->persistent) {
            if (::remove(filename.c_str()) != 0) {
                throw HDCException("MDBMStorage::cleanup(): Error deleting file\n");
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
        boost::property_tree::ptree root;
        std::stringstream ss;
        ss << settings;
        boost::property_tree::read_json(ss,root);
        init(root);
        return;
    }
    void init(boost::property_tree::ptree& root) {
        filename = root.get<std::string>("filename","/tmp/db.mdbm");
        persistent = root.get<bool>("persistent",true);
        D(printf("Filename: %s\n", filename.c_str());)
        this->db = mdbm_open(filename.c_str(), MDBM_O_RDWR | MDBM_O_CREAT | MDBM_LARGE_OBJECTS, 0666, 0, 0);
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
