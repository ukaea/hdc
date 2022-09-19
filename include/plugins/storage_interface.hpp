#ifndef STORAGE_INTERFACE
#define STORAGE_INTERFACE

#include <Pluma/Pluma.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

class Storage {
public:
    virtual ~Storage() = default;
    virtual void lock(boost::uuids::uuid uuid) = 0;
    virtual void unlock(boost::uuids::uuid uuid) = 0;
    virtual bool locked() = 0;
    virtual void sync() = 0;
    virtual std::string getDescription() = 0;
    virtual void set(boost::uuids::uuid uuid, char* data, size_t size) = 0;
    virtual char* get(boost::uuids::uuid uuid) = 0;
    virtual void cleanup() = 0;
    virtual size_t get_size(boost::uuids::uuid uuid) = 0;
    virtual bool has(boost::uuids::uuid uuid) = 0;
    virtual void remove(boost::uuids::uuid uuid) = 0;
    virtual void init(std::string settings) = 0;
    virtual std::string get_settings() = 0;
    virtual std::string name() = 0;
    virtual bool memory_mapped() = 0;
    virtual void reserve(boost::uuids::uuid uuid, size_t data_size) = 0;
};

PLUMA_PROVIDER_HEADER(Storage)

#endif // STORAGE_INTERFACE
