#ifndef HDC_GLOBAL_HPP
#define HDC_GLOBAL_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <boost/random.hpp>

// HDC
#include "hdc_storage.h"

class HDCGlobal {
public:
    //this holds all registered storages
    std::vector<HDCStorage*> stores;
    //this is default global storage
    HDCStorage* storage = nullptr;
    //list of found plugins
    std::unordered_map<std::string,std::string> avail_stores;
    boost::mt19937 ran;

    HDCGlobal() noexcept : stores{}, storage{ nullptr }, avail_stores{}, ran{} {};
};

#endif //HDC_GLOBAL_HPP
