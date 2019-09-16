#ifndef REDIS_PLUGIN_HPP
#define REDIS_PLUGIN_HPP
#include <hiredis/hiredis.h>
#include <string>
#include <iostream>
#include "storage_interface.hpp"
#include <iostream>
#include <cstdio>
#include <sstream>
#include <hdc_helpers.h>
#include <exception>
#include <json/json.h>
#include <vector>
#include <sys/time.h>
#include <unordered_map>

using namespace std;

class RedisStorageException: public std::exception {
private:
    std::string message_;
public:
    explicit RedisStorageException(const std::string& message);
    explicit RedisStorageException();
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};

class RedisStorage : public Storage {
private:
    redisContext* context;
    redisReply* reply;
    int port = 6379;
    std::string hostname = "127.0.0.1";
    bool use_unix_socket = false;
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    bool initialized = false;
    bool persistent = false;
    std::unordered_map<std::string,std::vector<char>> _cache;
public:
    RedisStorage() {
        DEBUG_STDOUT("RedisStorage()\n");
    };
    ~RedisStorage() {
        DEBUG_STDOUT("~RedisStorage()\n");
        if(!persistent) {
            cleanup();
        } else std::cout << "Storage has been set persistent. The data will can be preserved in redis instance" << std::endl;
    };
    void lock(string path) {
        std::cout << "Warning: Redis is locking by design. There is no need to call this explicitly..." << std::endl;
    };
    void unlock(string path) {
        std::cout << "Warning: Redis is locking by design. There is no need to call this explicitly..." << std::endl;
    };
    bool locked() {
        std::cout << "RedisStorage.locked(): Redis is locking by design. Hence returning false..." << std::endl;
        return false;
    };
    void sync() {};
    string getDescription() {
        return "This is MDBM based storage.";
    };
    std::string get_settings() {
        Json::Value root;
        root["persistent"] = persistent;
        root["filename"] = NULL;
        root["storage"] = "redis";
        std::stringstream ss;
        ss << root;
        return ss.str();
    };
    void set(string key, char* data, size_t size) {
//          if (_cache.find(key) != _cache.end()) _cache.erase(key);
//          _cache.emplace(key,vector<char>(data,data+size));
//         std::cout << "-- RedisStorage::set(" << key << ", " << size << ")\n";
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        this->reply = (redisReply*)redisCommand(this->context,"SET %b %b", key.c_str(), (size_t) key.size(), data, (size_t) size);
        if ( !this->reply ) throw ;//RedisStorageException("RedisStorage->set() has returned an error\n");
        freeReplyObject(this->reply);
        return;
    };
    char* get(string key) {
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        this->reply = (redisReply*)redisCommand(this->context,"GET %b", key.c_str(), (size_t) key.size());
        if ( !this->reply ) {
            throw ;//RedisStorageException("RedisStorage->get() has returned an error\n");
        } else {
            if ( this->reply->type != REDIS_REPLY_STRING ) {
                throw ;//RedisStorageException("RedisStorage->get() has returned wrong this->reply type\n");
            } else {
                if (_cache.find(key) != _cache.end())  _cache.erase(key);
                _cache.emplace(key,std::vector<char>(this->reply->len));
                std::copy(reply->str, reply->str+reply->len, _cache[key].begin());
                return &(_cache[key])[0];
            }
        }
    };
    size_t get_size(string key) {
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        this->reply = (redisReply*)redisCommand(this->context,"STRLEN %b", key.c_str(), (size_t) key.size());
        if ( !this->reply ) {
            throw;// RedisStorageException("RedisStorage->get() has returned an error\n");
        } else {
            if ( this->reply->type != REDIS_REPLY_INTEGER ) {
                throw;// RedisStorageException("RedisStorage->get() has returned wrong this->reply type\n");
            } else {
                size_t size = this->reply->integer;
                freeReplyObject(this->reply);
                return size;
            }
        }
    };
    void cleanup () {
        if (!initialized) return;
        DEBUG_STDOUT("RedisStorage::cleanup()\n");
        if (this->context != NULL) {
            redisFree(this->context);
        }
        _cache.clear();
        initialized = false;
        return;
    };
    bool has(string key) {
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        this->reply = (redisReply*)redisCommand(this->context,"EXISTS %b", key.c_str(), (size_t) key.size());
        if ( !this->reply ) {
            throw;// RedisStorageException("RedisStorage->get() has returned an error\n");
        } else {
            if ( this->reply->type != REDIS_REPLY_INTEGER ) {
                throw;// RedisStorageException("RedisStorage->get() has returned wrong this->reply type\n");
            } else {
                size_t exists = this->reply->integer;
                freeReplyObject(this->reply);
                return exists != 0;
            }
        }
    };
    void remove(string key) {
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        this->reply = (redisReply*)redisCommand(this->context,"DEL %b", key.c_str(), (size_t) key.size());
        if ( !this->reply ) {
            throw;// RedisStorageException("RedisStorage->get() has returned an error\n");
        } else {
            freeReplyObject(this->reply);
        }
        return;
    };
    void init(std::string settings) {
/*        Json::Value root;
        if (settings != "") {
            std::stringstream ss(settings);
            ss >> root;
        }
        persistent = root.get("persistent", false).asBool();
*/
        if (this->use_unix_socket) {
            this->context = redisConnectUnixWithTimeout(this->hostname.c_str(), this->timeout);
        } else {
            std::cout << "connect start\n";
            this->context = redisConnectWithTimeout(this->hostname.c_str(), this->port, this->timeout);
            std::cout << "connect done\n";
        }
        if (this->context == NULL || this->context->err) {
            if (this->context) {
                printf("Connection error: %s\n", this->context->errstr);
                redisFree(this->context);
            } else {
                printf("Connection error: can't allocate redis context\n");
            }
            exit(1);
        }
        initialized = true;
        return;
    };
    string name() {
        return "redis";
    };
    bool memory_mapped() {
        return false;
    };
};

PLUMA_INHERIT_PROVIDER(RedisStorage, Storage)

#endif //REDIS_PLUGIN_HPP
