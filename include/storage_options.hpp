#ifndef STORAGE_OPTIONS_HPP
#define STORAGE_OPTIONS_HPP

#include <string>

struct StorageOptions {
    std::string filename;
    bool persistent;
    bool do_init;
    StorageOptions(std::string _filename="", bool _persistent=false, bool _do_init=true) {
        filename = _filename;
        persistent = _persistent;
        do_init = _do_init;
    }
};

#endif //STORAGE_OPTIONS_HPP
