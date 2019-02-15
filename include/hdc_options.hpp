#ifndef HDC_OPTIONS_HPP
#define HDC_OPTIONS_HPP

#include <vector>
#include <string>
#include <unordered_map>

// HDC
#include "hdc_storage.h"


class HDCOptions {
public:
    //this holds all registered storages
    std::vector<HDCStorage> stores;
    //this is default global storage
    HDCStorage* global_storage;
    //list of found plugins
    std::unordered_map<std::string,std::string> avail_stores;
};

extern HDCOptions opt;

#endif //HDC_OPTIONS_HPP
