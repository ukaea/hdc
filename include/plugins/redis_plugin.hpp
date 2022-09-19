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
#include <boost/lexical_cast.hpp>

using namespace std;

class RedisStorage : public Storage {
private:
    redisContext* context;
    redisReply* reply;
    int port = 6379;
    std::string hostname = "127.0.0.1";
    bool use_unix_socket = false;
    struct timeval timeout = { 1, 500000 };
 // 1.5 seconds
    bool initialized = false;
    std::unordered_map<std::string, std::vector<char>> _cache;
public:
    RedisStorage() {
        DEBUG_STDOUT("RedisStorage()\n");
    }

    ~RedisStorage() {
        _cache.clear();
        DEBUG_STDOUT("~RedisStorage()\n");
    }

    void lock(boost::uuids::uuid _uuid) override {
//         std::cout << "Warning: Redis is locking by design. There is no need to call this explicitly..." << std::endl;
    }

    void unlock(boost::uuids::uuid _uuid) override {
//         std::cout << "Warning: Redis is locking by design. There is no need to call this explicitly..." << std::endl;
    }

    bool locked() override {
//         std::cout << "RedisStorage.locked(): Redis is locking by design. Hence returning false..." << std::endl;
        return false;
    }

    void sync()  override{}

    string getDescription() override {
        return "This is Redis based storage.";
    }

    std::string get_settings() override {
        Json::Value root;
        root["storage"] = "redis";
        root["hostname"] = hostname;
        root["port"] = port;
        root["use_unix_socket"] = use_unix_socket;
        root["persistent"] = true;
        std::stringstream ss;
        ss << root;
        return ss.str();
    }

    void reserve(boost::uuids::uuid _uuid, size_t data_size) override {
        if (!initialized) {
            throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        }
        auto key = boost::lexical_cast<std::string>(_uuid);
        this->reply = (redisReply*)redisCommand(this->context, "GET %b", key.c_str(), (size_t) key.size());
        if ( !this->reply ) {
            throw std::runtime_error("RedisStorage->reserve() [GET] has returned an error\n");
        } else if ( this->reply->type != REDIS_REPLY_STRING ) {
            throw std::runtime_error("RedisStorage->reserve() has returned wrong this->reply type\n");
        } else if (static_cast<size_t>(this->reply->len) > data_size) {
            throw std::runtime_error("RedisStorage->reserve(): invalid size\n");
        }
        auto new_data = std::make_unique<char[]>(data_size);
        memcpy(new_data.get(), this->reply->str, this->reply->len);
        freeReplyObject(this->reply);

        // Still not knowing why this is needed, but it makes the tests pass...
        this->reply = (redisReply*)redisCommand(this->context, "DEL %b", key.c_str(), (size_t)key.size());
        if ( !this->reply ) {
            throw std::runtime_error("RedisStorage->reserve() [DEL] has returned an error\n");
        }
        freeReplyObject(this->reply);
        // Actual SET
        this->reply = (redisReply*)redisCommand(this->context, "SET %b %b", key.c_str(), (size_t)key.size(), new_data.get(), data_size);
        if ( !this->reply ) {
            throw std::runtime_error("RedisStorage->reserve() [SET] has returned an error\n");
        }
        freeReplyObject(this->reply);
    }

    void set(boost::uuids::uuid _uuid, char* data, size_t size) override {
        if (!initialized) {
            throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        }
        auto key = boost::lexical_cast<std::string>(_uuid);
        // Still not knowing why this is needed, but it makes the tests pass...
        this->reply = (redisReply*)redisCommand(this->context, "DEL %b", key.c_str(), (size_t) key.size());
        if ( !this->reply ) throw std::runtime_error("RedisStorage->set() [DEL] has returned an error\n");
        freeReplyObject(this->reply);
        // Actual SET
        this->reply = (redisReply*)redisCommand(this->context, "SET %b %b", key.c_str(), (size_t) key.size(), data, (size_t) size);
        if ( !this->reply ) throw std::runtime_error("RedisStorage->set() [SET] has returned an error\n");
        freeReplyObject(this->reply);
    }

    char* get(boost::uuids::uuid _uuid) override {
        if (!initialized) {
            throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        }
        auto key = boost::lexical_cast<std::string>(_uuid);
        this->reply = (redisReply*)redisCommand(this->context, "GET %b", key.c_str(), (size_t) key.size());
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
    }

    size_t get_size(boost::uuids::uuid _uuid) override {
        if (!initialized) {
            throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        }
        auto key = boost::lexical_cast<std::string>(_uuid);
        this->reply = (redisReply*)redisCommand(this->context, "STRLEN %b", key.c_str(), (size_t) key.size());
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
    }

    void cleanup () override {
        if (!initialized) return;
        DEBUG_STDOUT("RedisStorage::cleanup()\n");
        if (this->context != NULL) {
            redisFree(this->context);
        }
        _cache.clear();
        initialized = false;
    }

    bool has(boost::uuids::uuid _uuid) override {
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        auto key = boost::lexical_cast<std::string>(_uuid);
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
    }

    void remove(boost::uuids::uuid _uuid) override {
        if (!initialized) throw std::runtime_error("RedisStorage: cannot perform action. init() has not been called...");
        auto key = boost::lexical_cast<std::string>(_uuid);
        this->reply = (redisReply*)redisCommand(this->context,"DEL %b", key.c_str(), (size_t) key.size());
        if ( !this->reply ) {
            throw std::runtime_error("RedisStorage->get() has returned an error\n");
        } else {
            freeReplyObject(this->reply);
        }
    }

    void init(std::string settings) override {
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
    }

    string name() override {
        return "redis";
    }

    bool memory_mapped() override {
        return false;
    }

};


PLUMA_INHERIT_PROVIDER(RedisStorage, Storage)

#endif //REDIS_PLUGIN_HPP
