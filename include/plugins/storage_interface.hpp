#ifndef STORAGE_INTERFACE
#define STORAGE_INTERFACE

#include <Pluma/Pluma.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace std;

class Storage{
public:
    virtual ~Storage() {}
    virtual void lock(string path) = 0;
    virtual void unlock(string path) = 0;
    virtual bool locked() = 0;
    virtual void sync() = 0;
    virtual bool usesBuffersDirectly() = 0;
    virtual string getDescription() = 0;
    virtual void set(string path, char* data, size_t size) = 0;
    virtual char* get(string path) = 0;
    virtual void cleanup() = 0;
    virtual size_t get_size(string path) = 0;
    virtual bool has(string path) = 0;
    virtual void remove(string path) = 0;
    virtual void init(string settings) = 0;
    virtual void init(boost::property_tree::ptree& settings) = 0;
    virtual string name() = 0;
};

PLUMA_PROVIDER_HEADER(Storage);

#endif // STORAGE_INTERFACE
