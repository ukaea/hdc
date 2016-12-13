#ifndef HDC_HPP
#define HDC_HPP

#define HDC_MEMSIZE_DEFAULT 5120
#define HDC_MEMSIZE_INCREMENT 5120

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
#include <json/json.h>

// our stuff
#include "types.h"
#include "hdc_map.h"
#include "utils.h"
#include "hdc_storage.h"

#define DEBUG

using namespace std;
template<typename T> struct identity { typedef T type; };

//this is default global storage
extern HDCStorage* global_storage;

/** Initializes global_storage  -- mainly due to C and Fortran */
void HDC_init(string pluginFileName="", string pluginSettingsFileName="");

/** Cleans up global_storage  -- mainly due to C and Fortran */
void HDC_destroy();

class HDC
{
private:
    string uuid;
    HDCStorage* storage;
    header_t header;

/* ------------------------------- methods ----------------------------------------- */
    void add_child(vector<string> vs, HDC* n);
    void set_child(vector<string> vs, HDC* n);
    void delete_child(vector<string> vs);
    HDC* get(vector<string> vs);
    HDC* get_slice(vector<string> vs, size_t i);
    bool has_child(vector<string> vs);

public:
    /** Creates empty HDC with specified buffer size */
    HDC(size_t byte_size);
    /** Default constructor. Creates empty HDC */
    HDC();
    /** Creates empty HDC with specified type and shape */
    HDC(int _ndim, size_t* _shape, size_t _type,Flags _flags = HDCDefault);
    /** Copy contructor */
    HDC(HDC* h);
    // Deserializing constructor
    HDC(HDCStorage* _storage, string _uuid);

    /** Destructor */
    ~HDC();
    size_t get_datasize();
    size_t get_size();
    /** Returns type of current node. */
    size_t get_type();
    size_t get_flags();
    template<typename T> T* get_data();
    
    template<typename T> void set_data(int _ndim, size_t* _shape, T* _data, Flags _flags = HDCDefault) {
        print("fixme\n");exit(7);
// //         #ifdef DEBUG
// //         printf("set_data(%d, {%d,%d,%d}, %f)\n",_ndim,_shape[0],_shape[1],_shape[2],((double*)_data)[0]);
// //         #endif
// //         if (storage->has(uuid)) {
// //             storage->remove(uuid);
// //         }
// //         type = to_typeid(_data[0]);
// //         size = hdc_sizeof(type);
// //         ndim = _ndim;
// //         memset(shape,0,HDC_MAX_DIMS*sizeof(size_t));
// //         for (int i = 0; i < _ndim; i++) {
// //             size *= _shape[i];
// //             shape[i] = _shape[i];
// //         }
// //         size_t buffer_size = size + HDC_DATA_POS;
// //         char* buffer = buff_allocate(buffer_size);
// //         buff_set_header(buffer,to_typeid(_data[0]),_flags,_ndim,_shape);
// //         memcpy(buffer+HDC_DATA_POS,_data,size);
// //         storage->set(uuid,buffer,buffer_size);
        return;
    }
    
    template<typename T> void set_data(int _ndim, initializer_list<size_t> _shape, T* _data, Flags _flags = HDCDefault) {
        set_data(_ndim,_shape,_data,_flags);
    };
    
    template<typename T> void set_data(string path, int _ndim, size_t* _shape, T* _data, Flags _flags = HDCDefault) {
        if(!has_child(path)) add_child(path, new HDC()); // TODO: add contructor for this!!
        get(path)->set_data(_ndim, _shape, _data, _flags);
    }
    
    template<typename T> void set_data(initializer_list<T> _data, Flags _flags = HDCDefault) {
        #ifdef DEBUG
        printf("template<typename T> void set_data(initializer_list<T> _data, Flags _flags = HDCDefault), %f\n",_data[0]);
        #endif
        vector<T> vec = _data;
        set_data(1,{vec.size()},&vec[0],_flags);
    };
    
    template<typename T> void set_data(string path, initializer_list<T> _data, Flags _flags = HDCDefault) {
        if(!has_child(path)) add_child(path, new HDC());
        get(path)->set_data(_data, _flags);
    }
    
    
    template<typename T> void set_data(string path, int _ndim, initializer_list<size_t> _shape, T* _data, Flags _flags = HDCDefault) {
        if(!has_child(path)) add_child(path, new HDC());
        get(path)->set_data(_ndim, _shape, _data, _flags);
    }
    
    /** Sets data to current node from vector<T> data. This function is primarily designed for interoperability with Python */
    template <typename T> void set_data(vector<T> data) 
    {
        #ifdef DEBUG
        printf("template <typename T> void set_data(vector<T> data), %f\n",data[0]);
        #endif
        if (get_children_ptr() != nullptr) {
            cout << "The node has already children set..." << endl;
            return;
        }
        size_t _shape[1] = {data.size()};
        set_data<T>(1,_shape,&data[0]);
        return;
    };

    /** Sets data to node on given path from vector<T> data. This function is primarily designed for interoperability with Python */
    template <typename T> void set_data(string path, vector<T> data) 
    {
        if (!this->has_child(path)) {
            this->add_child(path, new HDC());
            #ifdef DEBUG
            cout << "\"" << path << "\" not found, adding..." << endl;
            #endif
        }
        this->get(path)->set_data<T>(data);
        return;
    };

    void set_string(string str) {
        print("fixme\n");exit(7);
// //         size = str.length()+1;
// //         type = STRING_ID;
// //         ndim = 1;
// //         memset(shape,0,HDC_MAX_DIMS*sizeof(size_t));
// //         shape[0] = size;
// //         size_t buffer_size = size + HDC_DATA_POS;
// //         char* buffer = buff_allocate(buffer_size);
// //         buff_set_header(buffer,type,flags,ndim,shape);
// //         memcpy(buffer+HDC_DATA_POS,str.c_str(),size);
// //         storage->set(uuid,buffer,buffer_size);
    };
    void set_string(string path, string str) {
        if(!has_child(path)) add_child(path, new HDC()); // TODO: add constructor for this!!
        get(path)->set_string(str);
    }
    void set_data_c(int _ndim, size_t* _shape, void* _data, size_t _type);
    void set_data_c(string path, int _ndim, size_t* _shape, void* _data, size_t _type);
    /** Sets scalar data to given node. */
    template <typename T>
    void set_data(T data) {
        print("fixme\n");exit(7);
// //         type = to_typeid(data);
// //         size = sizeof(T);
// //         shape[0] = 0;
// //         ndim = 0;
// //         for (int i = 1; i<HDC_MAX_DIMS; i++) shape[i] = 0;
// //         char* buffer = buff_allocate(size + HDC_DATA_POS);
// //         buff_set_header(buffer,type,HDCDefault,1,shape);
// //         memcpy(buffer+HDC_DATA_POS,&data,sizeof(T));
// //         storage->set(uuid,buffer,size + HDC_DATA_POS);
    }
    template <typename T>
    void set_data(string path, T data) {
        if(!has_child(path)) add_child(path, new HDC());
        get(path)->set_data(data);
    }
    /** sets whole buffer */
    void set_buffer(char* buffer);
    /** Returns number of dimensions of current node. */
    int get_ndim();
    /** Returns shape of current node. */
    size_t* get_shape();
    bool is_external();
    bool is_readonly();
    bool is_fortranorder();
    void info();
/* -------------------------------- Old methods -- to be preserved ------------------------------- */
    /** Adds HDC subtree as child with given path. If neccessary, recursively creates subnodes. */
    void add_child(string path, HDC* n);
    /** Sets HDC subtree to given path. */
    void set_child(string path, HDC* n);
    /** Deletes HDC subtree. */
    void delete_child(string path);
    /** Returns subtree by path. */
    HDC* get(string path);
    /** Returns i-th subnode if HDC_LIST is the type. */
    HDC* get_slice(string path, size_t i); 
    /** Returns i-th subnode if HDC_LIST is the type. */
    HDC* get_slice(size_t i); 
    /** Returns true if subtree with given path with exists and false otherwise. */
    bool has_child(string path);
    
    HDC* json_to_hdc(Json::Value* root);

    /** Sets HDC_LIST from std::deque<HDC*> data.*/
    void set_list(deque<HDC*>* list); 
    /** Performs deep copy of current node if recursively = 1. Performs shallow copy otherwise. */
    void resize(HDC* h, int recursively = 0); 
    /** Returns copy of current object. */
    HDC* copy(int copy_arrays = 1); 
    /** Inserts node to i-th slice of current node. */
    void insert_slice(size_t i, HDC* h);
    /** Sets node to i-th slice of current node. */
    void set_slice(size_t i, HDC* h);
    /** Appends given node as next available slice (similar to push_back() method seen in C++ STL containers).*/
    void append_slice(HDC* h); 
    /** Sets HDC type of current node. */
    void set_type(size_t _type);
    /** Returns true if node is empty. */
    bool is_empty();
    /** Returns number of dimensions of node under path. */
    int get_ndim(string path);
    /** Returns shape of node under path. */
    size_t* get_shape(string path);
    /** Returns pointer to data of this node. */
    template<typename T> T as()
    {
        if (get_children_ptr() != nullptr) {
            cout << "This node is not terminal" << endl;
            return reinterpret_cast<T>(0);
        }
        #ifdef DEBUG
//         cout << "From as:" << data[0] << endl;
        #endif
        if (!storage->has(uuid)) {
            printf("Not found: %s\n",uuid.c_str());
            exit(-3);
        }
        print("fixme\n");exit(7);
// //         return reinterpret_cast<T>(buff_get_data_ptr(storage->get(uuid)));
        return reinterpret_cast<T>(0);
    }
    /** Returns string. Needs to have separate function */
    std::string as_string() {
        if (header.type == STRING_ID) {
print("fixme\n");exit(7);
// //             string str(buff_get_data_ptr(storage->get(uuid)));
            string str("fixme");
            return str;
        } else return "as_string(): Not implemented yet for give type.";
    }
    
    /** Returns string of node under given path. Needs to have separate function */
    std::string as_string(string path)
    {
        #ifdef DEBUG
        printf("as_string(%s)\n",path.c_str());
        #endif
        return get(path)->as_string();
    }
    
    /** Returns pointer to data of node under given path. */
    template<typename T> T as(string path)
    {
        #ifdef DEBUG
        printf("as<T>(%s)\n",path.c_str());
        #endif
        return get(path)->as<T>();
    }
    
    /** Returns double. */
    double as_double() 
    {
        return as<double*>()[0];
    }
    /** Returns double. */
    double as_double(string path) 
    {
        return as<double*>(path)[0];
    }
    /** Returns pointer to self. */
    hdc_t* as_hdc_ptr();
    /** Serialization to JSON file. */
    void to_json(string filename, int mode = 0);
    /** Serialization to Json::Value object. */
    Json::Value to_json(int mode = 0);
    /** Dumps JSON to cout */
    void dump();
    /** Returns void pointer to data. */
    void* as_void_ptr();
    /** Returns string representing data/node type. */
    string get_type_str();
    /** Returns datashape desctiption string. */
    string get_datashape_str();
    /** Returns string representing data/node type. */
    string get_type_str(string path);
    /** Returns datashape desctiption string. */
    string get_datashape_str(string path);
    /** Returns vector of keys of a struct node and empty vector otherwise. */
    vector<string> keys();
    size_t childs_count();
    char* get_buffer();
    string get_uuid();

    // allocator stuff
    bip::managed_external_buffer get_segment();
    map_t* get_children_ptr();

    // flags set/get
//     bool get_flag(uint pos);
//     void set_flag(uint pos, bool val);
};

HDC* from_json(const string& filename); //todo: make constructor from this
string map_to_json(map_t& children);

#endif // HDC_HPP
