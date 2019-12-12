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

/**
 * Check if a file exists
 * @return true if and only if the file exists, false else
 */
bool MDBMFileExists(const std::string& file) {
    struct stat buf = {};
    return (stat(file.c_str(), &buf) == 0);
}

class MDBMStorage : public Storage {
private:
    MDBM* db = nullptr;
    bool initialized = false;
    bool persistent = false;
    std::string filename = "/tmp/db.mdbm";

public:
    MDBMStorage() {
        DEBUG_STDOUT("MDBMStorage()\n");
    }

    ~MDBMStorage() override {
        DEBUG_STDOUT("~MDBMStorage()\n");
        if(!persistent) {
            cleanup();
        } else std::cout << "Storage has been set persistent. The data are stored in file \"" << filename << "\""<< std::endl;
    }

    void lock(boost::uuids::uuid _uuid) override {
        if (!initialized) throw std::runtime_error("MDBM: cannot perform action. init() has not been called...");
        datum key = { (char*)(&_uuid), _uuid.size() };
        mdbm_lock_smart (this->db, &key, 0);
    }

    void unlock(boost::uuids::uuid _uuid) override {
        if (!initialized) throw std::runtime_error("MDBM: cannot perform action. init() has not been called...");
        datum key = { (char*)(&_uuid), _uuid.size() };
        mdbm_unlock_smart (this->db, &key, 0);
    }

    bool locked() override {
        return mdbm_islocked(this->db);
    }

    void sync() override {}

    std::string getDescription() override {
        return "This is MDBM based storage.";
    }

    std::string get_settings() override {
        Json::Value root;
        root["persistent"] = persistent;
        root["filename"] = filename;
        root["storage"] = "mdbm";
        std::stringstream ss;
        ss << root;
        return ss.str();
    }

    void set(boost::uuids::uuid _uuid, char* data, size_t size) override {
        if (!initialized) {
            throw std::runtime_error("MDBM: cannot perform action. init() has not been called...");
        }
        datum key = { (char*)(&_uuid), _uuid.size() };
        datum value = {(char*)data, static_cast<int>(size)};
        mdbm_lock_smart (this->db, &key, 0);
        mdbm_store(this->db,key,value,MDBM_REPLACE);
        mdbm_unlock_smart (this->db, &key, 0);
    }

    char* get(boost::uuids::uuid _uuid) override {
        if (!initialized) {
            throw std::runtime_error("MDBM: cannot perform action. init() has not been called...");
        }
        datum key = { (char*)(&_uuid), _uuid.size() };
        datum found = mdbm_fetch(this->db, key);
        if (found.dptr == NULL || found.dsize == 0) {
            throw std::runtime_error("MDBMStorage::get(\""+boost::lexical_cast<std::string>(_uuid)+"\"): not found\n");
        }
        return (char*) found.dptr;
    }

    size_t get_size(boost::uuids::uuid _uuid) override {
        if (!initialized) {
            throw std::runtime_error("MDBM: cannot perform action. init() has not been called...");
        }
        datum key = { (char*)(&_uuid), _uuid.size() };
        datum found = mdbm_fetch(this->db, key);
        return found.dsize;
    }

    void cleanup () override {
        if (!initialized) return;
        DEBUG_STDOUT("MDBMStorage::cleanup()\n");
        if (this->db != nullptr) {
            mdbm_close(this->db);
            mdbm_delete_lockfiles(this->filename.c_str());
            this->db = nullptr;
        }
        // Remove db file if the data persistence is not required
        if (!this->persistent && MDBMFileExists(filename)) {
            if (::remove(filename.c_str()) != 0) {
                throw std::runtime_error("MDBMStorage::cleanup(): Error deleting file\n");
            };
        }
        initialized = false;
    }

    bool has(boost::uuids::uuid _uuid) override {
        if (!initialized) {
            throw std::runtime_error("MDBM: cannot perform action. init() has not been called...");
        }
        datum key = { (char*)(&_uuid), _uuid.size() };
        return (mdbm_fetch(this->db, key).dsize != 0);
    }

    void remove(boost::uuids::uuid _uuid) override {
        if (!initialized) {
            throw std::runtime_error("MDBM: cannot perform action. init() has not been called...");
        }
        datum key = { (char*)(&_uuid), _uuid.size() };
        mdbm_delete(this->db,key);
    }

    void init(std::string settings) override {
        Json::Value root;
        if (!settings.empty()) {
            std::stringstream ss(settings);
            ss >> root;
        }
        filename = root.get("filename", "/tmp/db.mdbm").asString();
        persistent = root.get("persistent", false).asBool();
        D(printf("Filename: %s\n", filename.c_str());)
        this->db = mdbm_open(filename.c_str(), MDBM_O_RDWR | MDBM_O_CREAT | MDBM_LARGE_OBJECTS, 0666, 0, 0);
        mdbm_set_alignment(this->db,MDBM_ALIGN_16_BITS);
        initialized = true;
    }

    std::string name() override {
        return "mdbm";
    }
};

PLUMA_INHERIT_PROVIDER(MDBMStorage, Storage)

#endif //MDBM_PLUGIN_HPP
