#ifndef HDC_HPP
#define HDC_HPP

#include <iostream>

// marray - array views
#include "andres/marray.hxx"

// Boost
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

// some other stuff -- to be reduced later
#include <cstdint>
#include <vector>
#include <deque>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <json/json.h>
#include <exception>

// our stuff
#include "hdc_types.h"
#include "hdc_map.h"
#include "hdc_utils.h"
#include "hdc_storage.h"
#include "hdc_errors.hpp"

#ifdef _USE_HDF5
#include <H5Cpp.h>
#endif

// #define DEBUG

#include "hdc_helpers.h"
#include <unordered_map>

using namespace std;
namespace pt = boost::property_tree;

//this will hold all the settings, instead of boost::property_tree json we will use the jsoncpp for actual reading of them.
extern pt::ptree* options;
//this is default global storage
extern HDCStorage* global_storage;
//list of found plugins
extern unordered_map<string,string> avail_stores;

using byte = unsigned char;

class HDC
{
private:
    string uuid;
    HDCStorage* storage;

/* ------------------------------- methods ----------------------------------------- */
    void add_child(vector<boost::variant<size_t,std::string>> vs, HDC* n);
    void add_child(vector<boost::variant<size_t,std::string>> vs, HDC& n);
    void set_child(vector<boost::variant<size_t,std::string>> vs, HDC* n);
    void delete_child(vector<boost::variant<size_t,std::string>> vs);
    HDC* get_ptr(vector<boost::variant<size_t,std::string>> vs);
    HDC get(vector<boost::variant<size_t,std::string>> vs);
    HDC get_slice(vector<boost::variant<size_t,std::string>> vs, size_t i);
    HDC* get_slice_ptr(vector<boost::variant<size_t,std::string>> vs, size_t i);
    bool exists(vector<boost::variant<size_t,std::string>> vs) const;
    bool exists_single(boost::variant<size_t,std::string> index) const;
    void add_child_single(const std::string& path, HDC& n);
    hdc_header_t get_header() const;
//     void add_child_single(const boost::variant<size_t,std::string>& path, HDC& n);
public:
    /** Creates empty HDC with specified buffer size */
    HDC(size_t byte_size);
    /** Default constructor. Creates empty HDC */
    HDC();
    /** Creates empty HDC with specified type and shape */
    HDC(int _rank, size_t* _shape, hdc_type_t _type,long _flags = HDCDefault);
    /** Constructor from string */
    HDC(const std::string str);
    /** Copy contructor */
    HDC(HDC* h);
    /** Deserializing constructor */
    HDC(HDCStorage* _storage, const std::string& _uuid);
    /** constructor from object buffer -- this should be void* as we want cha* to be used for strings */
    HDC(byte* src_buffer);
    /** Destructor */
    ~HDC();
    /** Parses command line arguments */
    static void parse_cmdline(int argc, const char *argv[]);
    /** Loads settings from file */
    static void load_config(std::string configPath="./hdc.conf:~/.config/hdc.conf");
    /** Searches for available storage plugins */
    static void search_plugins(string searchPath="./:./plugins:./hdc_plugins:.local/hdc/plugins");
    /** Prints out all found storage plugins. */
    static void list_plugins();
    /** Sets the default storage to be used. */
    static void set_storage(std::string storage="umap");
    /** Returns list of all available storage plugins */
    static std::vector<std::string> get_available_plugins();
    /** Returns a directory path where the HDC library is loaded from.  */
    static std::string get_library_dir(void);
    /** Initializes global_storage  -- mainly due to C and Fortran */
    static void init(std::string storage="umap",std::string storage_options="");
    /** Sets the default storage options, builds the boost::property_tree with default settings structure to be redefined later by parse_cmdline(), load_config() or set_storage() functions. */
    static void set_default_storage_options(std::string storage="umap", std::string storage_options="");
    /** Cleans up global_storage  -- mainly due to C and Fortran */
    static void destroy();
    /** Returns the available space in buffer (in bytes) */

    size_t get_datasize() const;
    /** Returns the the size of object buffer (= header+data, in bytes) */
    size_t get_size() const;
    /** Returns type of current node. */
    size_t get_type() const;
    /** Returns the size of a single item in bytes */
    size_t get_itemsize() const;
    /** Returns object flags (i.e. array ordering)*/
    size_t get_flags() const;
    /** Returns the data, the pointer is just casted => there is no conversion for now.*/
    template<typename T> T* get_data() const;
    /** Stores data in node's buffer */
    template<typename T> void set_data(int _rank, size_t* _shape, T* _data, hdc_flags_t _flags = HDCDefault) {
        hdc_header_t header = get_header();
        D(printf("set_data(%d, {%d,%d,%d}, %f)\n",_rank,_shape[0],_shape[1],_shape[2],((double*)_data)[0]);)
        auto buffer = storage->get(uuid);
        memcpy(&header,buffer,sizeof(hdc_header_t));
        // Start with determining of the buffer size
        size_t data_size = sizeof(T);
        for (int i=0;i<_rank;i++) data_size *= _shape[i];
        size_t buffer_size = data_size + sizeof(hdc_header_t);
        if (header.buffer_size == buffer_size) {
            storage->lock(uuid);
            memcpy(buffer+sizeof(hdc_header_t),_data,data_size);
            storage->unlock(uuid);
            return;
        } else {
            header.buffer_size = buffer_size;
            header.data_size = data_size;
            memset(header.shape,0,HDC_MAX_DIMS*sizeof(size_t));
            for (int i=0;i<_rank;i++) header.shape[i] = _shape[i];
            header.flags = _flags;
            header.type = to_typeid(_data[0]);
            header.rank = _rank;
            char* buffer = new char[header.buffer_size];
            memcpy(buffer,&header,sizeof(hdc_header_t));
            memcpy(buffer+sizeof(hdc_header_t),_data,header.data_size);
            storage->set(uuid,buffer,header.buffer_size);
            if (!storage->usesBuffersDirectly()) delete[] buffer;
            return;
        }
    }

    template<typename T> void set_data(int _rank, initializer_list<size_t> _shape, T* _data, hdc_flags_t _flags = HDCDefault) {
        set_data(_rank,_shape,_data,_flags);
    };

    template<typename T> void set_data(const std::string& path, int _rank, size_t* _shape, T* _data, hdc_flags_t _flags = HDCDefault) {
        if(!exists(path)) {
            HDC h;
            add_child(path, h); // TODO: add contructor for this!!
        }
        get(path).set_data(_rank, _shape, _data, _flags);
    }

    template<typename T> void set_data(initializer_list<T> _data, hdc_flags_t _flags = HDCDefault) {
        DEBUG_STDOUT("template<typename T> void set_data(initializer_list<T> _data, hdc_flags_t _flags = HDCDefault)"+to_string(_data[0]));
        vector<T> vec = _data;
        set_data(1,{vec.size()},&vec[0],_flags);
    };

    template<typename T> void set_data(const std::string& path, initializer_list<T> _data, hdc_flags_t _flags = HDCDefault) {
        if(!exists(path)) {
            HDC h;
            add_child(path, h);
        }
        get(path).set_data(_data, _flags);
    }


    template<typename T> void set_data(const std::string& path, int _rank, initializer_list<size_t> _shape, T* _data, hdc_flags_t _flags = HDCDefault) {
        if(!exists(path)) {
            HDC h;
            add_child(path, h);
        }
        get(path).set_data(_rank, _shape, _data, _flags);
    }

    /** Sets data to current node from vector<T> data. This function is primarily designed for interoperability with Python */
    template <typename T> void set_data(vector<T> data)
    {
        DEBUG_STDOUT("template <typename T> void set_data(vector<T> data)"+to_string(data[0]));
        if (get_children_ptr() != nullptr) {
            cout << "The node has already children set..." << endl;
            return;
        }
        size_t _shape[1] = {data.size()};
        set_data<T>(1,_shape,&data[0]);
        return;
    };

    /** Sets data to node on given path from vector<T> data. This function is primarily designed for interoperability with Python */
    template <typename T> void set_data(const std::string& path, vector<T> data)
    {
        if (!this->exists(path)) {
            HDC h;
            this->add_child(path, h);
            DEBUG_STDOUT("\""+path+"\" not found, adding...\n");
        }
        get(path).set_data<T>(data);
        return;
    };

    void set_string(std::string str) {
        hdc_header_t header = get_header();
        if (storage->has(uuid)) {
            storage->remove(uuid);
        }
        memset(&header,0,sizeof(hdc_header_t));
        header.data_size = str.length()+1;
        header.type = HDC_STRING;
        header.rank = 1;
        header.shape[0] = str.length();
        header.buffer_size = header.data_size + sizeof(hdc_header_t);
        auto buffer = new char[header.buffer_size];
        memcpy(buffer,&header,sizeof(hdc_header_t));
        memcpy(buffer+sizeof(hdc_header_t),str.c_str(),header.data_size);
        storage->set(uuid,buffer,header.buffer_size);
        if (!storage->usesBuffersDirectly()) delete[] buffer;
    };

    void set_string(const std::string& path, const std::string& str) {
        if(!path.empty() && !exists(path)) {
            HDC h;
            add_child(path, h); // TODO: add constructor for this!!
        }
        if (!path.empty()) get(path).set_string(str);
        else set_string(str);
    }

    void set_string(std::vector <boost::variant<size_t,std::string>> path, const std::string& str) {
        if(!path.empty() && !exists(path)) {
            HDC h;
            add_child(path, h); // TODO: add constructor for this!!
        }
        if (!path.empty()) get(path).set_string(str);
        else set_string(str);
    }
    void set_data_c(int _rank, size_t* _shape, void* _data, hdc_type_t _type, hdc_flags_t _flags = HDCDefault);
    void set_data_c(const std::string& path, int _rank, size_t* _shape, void* _data, hdc_type_t _type, hdc_flags_t _flags = HDCDefault);
    void set_data_c(int _rank, size_t* _shape, const void* _data, hdc_type_t _type, hdc_flags_t _flags = HDCDefault);
    void set_data_c(const std::string& path, int _rank, size_t* _shape, const void* _data, hdc_type_t _type, hdc_flags_t _flags = HDCDefault);
    void set_data_c(vector<boost::variant<size_t,std::string>> path, int _rank, size_t* _shape, const void* _data, hdc_type_t _type, hdc_flags_t _flags = HDCDefault);
    /** Sets scalar data to given node. */
    template <typename T>
    void set_data(T data) {
        hdc_header_t header = get_header();
        memset(&header,0,sizeof(hdc_header_t));
        header.type = to_typeid(data);
        header.data_size = sizeof(T);
        header.buffer_size = header.data_size + sizeof(hdc_header_t);
        char* buffer = new char[header.buffer_size];
        memcpy(buffer,&header,sizeof(hdc_header_t));
        memcpy(buffer+sizeof(hdc_header_t),&data,header.data_size);
        storage->set(uuid,buffer,header.buffer_size);
        if (!storage->usesBuffersDirectly()) delete[] buffer;
    }
    /** Sets scalar data to given node - UDA version. */
    void set_data(const unsigned char* data, hdc_type_t _type) {
        hdc_header_t header = get_header();
        header.type = _type;
        header.data_size = hdc_sizeof(_type);
        header.buffer_size = header.data_size + sizeof(hdc_header_t);
        char* buffer = new char[header.buffer_size];
        memcpy(buffer,&header,sizeof(hdc_header_t));
        memcpy(buffer+sizeof(hdc_header_t),data,header.data_size);
        storage->set(uuid,buffer,header.buffer_size);
        if (!storage->usesBuffersDirectly()) delete[] buffer;
    }
    /** Sets scalar data to given node - UDA version. */
    void set_data(void* data, hdc_type_t _type) {
        return this->set_data((const unsigned char*)data,_type);
    }
    /** Sets scalar data to given node - UDA version with path. */
    void set_data(const std::string path, const unsigned char* data, hdc_type_t _type) {
        if (path.empty()) {
            set_data(data, _type);
        } else {
            if(!exists(path)) {
                HDC h;
                add_child(path, h); // TODO: add constructor for this!!
            }
            get(path).set_data(data, _type);
        }
    }
    /** Sets scalar data to given node - UDA version with path. */
    void set_data(const std::string path, void* data, hdc_type_t _type) {
        if (path.empty()) {
            set_data(data, _type);
        } else {
            if(!exists(path)) {
                HDC h;
                add_child(path, h); // TODO: add constructor for this!!
            }
            get(path).set_data(data, _type);
        }
    }
    template <typename T>
    void set_data(const std::string& path, T data) {
        if(!exists(path)) {
            HDC h;
            add_child(path, h);
        }
        get(path).set_data(data);
    }

    void set_data(const std::string& path, const unsigned char* data, hdc_type_t _type) {
        if(!exists(path)) {
            HDC h;
            add_child(path, h);
        }
        get(path).set_data(data,_type);
    }
    /** Returns number of dimensions of current node. */
    int get_rank() const;
    /** Returns shape of current node. */
    size_t* get_shape() const;
    std::vector<size_t> get_strides() const;
    bool is_external() const;
    bool is_readonly() const;
    bool is_fortranorder() const;
    void print_info() const;
/* -------------------------------- Old methods -- to be preserved ------------------------------- */
    /** Adds HDC subtree as child with given path. If neccessary, recursively creates subnodes. Pointer version. */
    void add_child(const std::string& path, HDC* n);
    /** Adds HDC subtree as child with given path. If neccessary, recursively creates subnodes. Reference version. */
    void add_child(const std::string& path, HDC& n);
    /** Sets HDC subtree to given path. */
    void set_child(const std::string& path, HDC* n);
    /** Deletes HDC subtree. */
    void delete_child(const std::string& path);
    /** Returns subtree by path. */
    HDC* get_ptr(const std::string& path);
    HDC get(const std::string& path);
    /** Returns i-th subnode if HDC_LIST is the type. */
    HDC get_slice(const std::string& path, size_t i);
    /** Returns i-th subnode if HDC_LIST is the type. */
    HDC get_slice(size_t i);
    HDC* get_slice_ptr(const std::string& path, size_t i);
    /** Returns i-th subnode if HDC_LIST is the type. */
    HDC* get_slice_ptr(size_t i);
    /** Returns true if subtree with given path with exists and false otherwise. */
    bool exists(const std::string& path) const;
    /** Sets HDC_LIST from std::deque<HDC*> data.*/
    void set_list(deque<HDC*>* list);
    /** Performs deep copy of current node if recursively = 1. Performs shallow copy otherwise. */
    void resize(HDC* h, int recursively = 0);
    /** Returns copy of current object. */
    HDC* copy(int copy_arrays = 1);
    /** Inserts node to i-th slice of current node. */
    void insert_slice(size_t i, HDC* h);
    /** Inserts node to i-th slice of current node. */
    void insert_slice(size_t i, HDC& h);
    /** Sets node to i-th slice of current node. */
    void set_slice(size_t i, HDC* h);
    /** Sets node to i-th slice of current node. */
    void set_slice(size_t i, HDC& h);
    /** Appends given node as next available slice (similar to push_back() method seen in C++ STL containers).*/
    void append_slice(HDC* h);
    /** Appends given node as next available slice (similar to push_back() method seen in C++ STL containers).*/
    void append_slice(HDC& h);
    /** Sets HDC type of current node. */
    void set_type(hdc_type_t _type);
    /** Returns true if node is empty. */
    bool is_empty() const;
    /** Returns number of dimensions of node under path. */
    int get_rank(const std::string& path);
    /** Returns shape of node under path. */
    size_t* get_shape(const std::string& path);
    /** Returns pointer to data of this node. */
    template<typename T> T as()
    {
        hdc_header_t header = get_header();
        if (header.type == HDC_STRUCT || header.type == HDC_LIST) {
            throw std::runtime_error("This is not a terminal node...");
        }
        DEBUG_STDOUT("as<"+get_type_str()+">()");
        if (!storage->has(uuid)) {
            throw HDCException("as(): Not found: "+std::string(uuid.c_str())+"\n");
        }
        return reinterpret_cast<T>(storage->get(uuid)+sizeof(hdc_header_t));
    }
    /** Returns string. Needs to have separate function */
    std::string as_string() {
        hdc_header_t header = get_header();
        if (header.type == HDC_STRING) {
           std::string str(storage->get(uuid)+sizeof(hdc_header_t));
           return str;

        } else {
            cout << header.type << endl;
            std::ostringstream oss;
            oss << to_json(0) << "\n";
            return oss.str();
        }
    }

    /** Returns string of node under given path. Needs to have separate function */
    std::string as_string(const std::string& path)
    {
        DEBUG_STDOUT("as_string("+path+")\n");
        if (path.empty()) {
            std::cerr << "DD::\n";
            return as_string();
        }
        else {
            std::cerr << "EE::\n";
            return get(path).as_string();
        }
    }

    /** Returns pointer to data of node under given path. */
    template<typename T> T as(const std::string& path)
    {
        DEBUG_STDOUT("as<T>("+path+")\n");
        if (path.empty()) return as<T>();
        else return get(path).as<T>();
    }

    /** Returns double. */
    double as_double()
    {
        return as<double*>()[0];
    }
    /** Returns double. */
    double as_double(const std::string& path)
    {
        return as<double*>(path)[0];
    }
    /** Returns pointer to self. */
    hdc_t* as_hdc_ptr();
    /** Serialization to JSON file. */
    void to_json(string filename, int mode = 0);
    /** Serialization to Json::Value object. */
    Json::Value to_json(int mode = 0);
    /** Serialization to string object. */
    string to_json_string(int mode = 0);
    /** Dumps JSON to cout */
    void dump();
    /** Serializes HDC to special json file*/
    string serialize();
    void serialize(string filename);
    /** Returns void pointer to data. */
    intptr_t as_void_ptr();
    /** Returns string representing data/node type. */
    const char* get_type_str() const;
    /** Returns datashape desctiption string. */
    string get_datashape_str() const;
    /** Returns string representing data/node type. */
    string get_type_str(const std::string& path);
    /** Returns datashape desctiption string. */
    string get_datashape_str(const std::string& path);
    /** Returns void pointer to data */
    char* get_data_ptr() const;
    /** Returns vector of keys of a struct node and empty vector otherwise. */
    vector<string> keys() const;
    size_t childs_count() const;
    char* get_buffer() const;
    string get_uuid() const;
    void grow(size_t extra_size);
    // allocator stuff
    bip::managed_external_buffer get_segment() const;
    hdc_map_t* get_children_ptr() const;
    void delete_data();
    static HDC from_uda(const std::string& signalName, const std::string& dataSource, bool withMetadata = false);
    // "static contructor" from void* HDC
    static HDC* new_HDC_from_cpp_ptr(intptr_t cpp_ptr);
    // "static contructor" from hdc_t*
    static HDC* new_HDC_from_c_ptr(intptr_t c_ptr);
    // "deserialize from storage"
    static HDC* deserialize_HDC_file(const std::string& filename);
    // "deserialize from storage"
    static HDC* deserialize_HDC_string(const std::string& filename);
    static HDC from_json(const string& filename, const std::string& datapath = "");
    static string hdc_map_to_json(hdc_map_t& children);
    static char* buffer_grow(char* old_buffer, size_t extra_size);
    static HDC json_to_HDC(const ::Json::Value& root);
    static HDC from_json_string(const std::string& json_string);
    void to_hdf5(std::string filename, std::string dataset_name = "data");
    static HDC from_hdf5(const std::string& filename, const std::string& dataset_name = "/data");
    static HDC* from_hdf5_ptr(const std::string& filename, const std::string& dataset_name = "/data");
    static HDC uda2HDC(const std::string& data_object, const std::string& data_source);
    static HDC load(const std::string& str, const std::string& datapath="");
    HDCStorage* get_storage() const {return this->storage; };

    hdc_data_t get_data();
    hdc_data_t get_data(const std::string& path);
    void set_data(hdc_data_t obj);
    void set_data(const std::string& path,hdc_data_t obj);

};

#endif // HDC_HPP
