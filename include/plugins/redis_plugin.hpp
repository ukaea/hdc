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

class RedisStorage : public Storage {
private:
    redisContext* context;
    redisReply* reply;
    int port = 6379;
    std::string hostname = "127.0.0.1";
    bool use_unix_socket = false;
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    bool initialized = false;
    std::unordered_map<std::string,std::vector<char>> _cache;
public:
    RedisStorage() {
        DEBUG_STDOUT("RedisStorage()\n");
    };
    ~RedisStorage() {
        _cache.clear();
        DEBUG_STDOUT("~RedisStorage()\n");
    };
    void lock(string path) {
//         std::cout << "Warning: Redis is locking by design. There is no need to call this explicitly..." << std::endl;
    };
    void unlock(string path) {
//         std::cout << "Warning: Redis is locking by design. There is no need to call this explicitly..." << std::endl;
    };
    bool locked() {
//         std::cout << "RedisStorage.locked(): Redis is locking by design. Hence returning false..." << std::endl;
        return false;
    };
    void sync() {};
    string getDescription() {
        return "This is Redis based storage.";
    };
    std::string get_settings() {
        Json::Value root;
        root["storage"] = "redis";
        root["hostname"] = hostname;
        root["port"] = port;
        root["use_unix_socket"] = use_unix_socket;
        std::stringstream ss;
        ss << root;
        return ss.str();
    };
    void set(string key, char* data, size_t size) {
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        this->reply = (redisReply*)redisCommand(this->context,"SET %b %b", key.c_str(), (size_t) key.size(), data, (size_t) size);
        if ( !this->reply ) throw std::runtime_error("RedisStorage->set() has returned an error\n");
        freeReplyObject(this->reply);
        return;
    };
    char* get(string key) {
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        this->reply = (redisReply*)redisCommand(this->context,"GET %b", key.c_str(), (size_t) key.size());
        if ( !this->reply ) {
            throw std::runtime_error("RedisStorage->get() has returned an error\n");
        } else {
            if ( this->reply->type != REDIS_REPLY_STRING ) {
                throw std::runtime_error("RedisStorage->get() has returned wrong this->reply type\n");
            } else {
                if (_cache.find(key) != _cache.end())  _cache.erase(key);
                _cache.emplace(key,std::vector<char>(this->reply->len));
                std::copy(reply->str, reply->str+reply->len, _cache[key].begin());
                freeReplyObject(this->reply);
                return &(_cache[key])[0];
            }
        }
    };
    size_t get_size(string key) {
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        this->reply = (redisReply*)redisCommand(this->context,"STRLEN %b", key.c_str(), (size_t) key.size());
        if ( !this->reply ) {
            throw std::runtime_error("RedisStorage->get() has returned an error\n");
        } else {
            if ( this->reply->type != REDIS_REPLY_INTEGER ) {
                throw std::runtime_error("RedisStorage->get() has returned wrong this->reply type\n");
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
    };
    bool has(string key) {
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        this->reply = (redisReply*)redisCommand(this->context,"EXISTS %b", key.c_str(), (size_t) key.size());
        if ( !this->reply ) {
            throw std::runtime_error("RedisStorage->get() has returned an error\n");
        } else {
            if ( this->reply->type != REDIS_REPLY_INTEGER ) {
                throw std::runtime_error("RedisStorage->get() has returned wrong this->reply type\n");
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
            throw std::runtime_error("RedisStorage->get() has returned an error\n");
        } else {
            freeReplyObject(this->reply);
        }
    };
    void init(std::string settings) {
        Json::Value root;
        if (settings != "") {
            std::stringstream ss(settings);
            ss >> root;
        }
        hostname = root.get("hostname", "127.0.0.1").asString();
        port = root.get("port", 6379).asInt();
        use_unix_socket = root.get("use_unix_socket",false).asBool();

        if (this->use_unix_socket) {
            this->context = redisConnectUnixWithTimeout(this->hostname.c_str(), this->timeout);
        } else {
            this->context = redisConnectWithTimeout(this->hostname.c_str(), this->port, this->timeout);
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
