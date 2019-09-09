#ifndef STORAGE_INTERFACE
#define STORAGE_INTERFACE

#include <Pluma/Pluma.hpp>

class Storage {
public:
    virtual ~Storage() = default;
    virtual void lock(std::string path) = 0;
    virtual void unlock(std::string path) = 0;
    virtual bool locked() = 0;
    virtual void sync() = 0;
    virtual std::string getDescription() = 0;
    virtual void set(std::string path, char* data, size_t size) = 0;
    virtual char* get(std::string path) = 0;
    virtual void cleanup() = 0;
    virtual size_t get_size(std::string path) = 0;
    virtual bool has(std::string path) = 0;
    virtual void remove(std::string path) = 0;
    virtual void init(std::string settings) = 0;
    virtual std::string get_settings() = 0;
    virtual std::string name() = 0;
};

PLUMA_PROVIDER_HEADER(Storage)

#endif // STORAGE_INTERFACE
