#ifndef HDC_HPP
#define HDC_HPP

#include <iostream>

// multi_index container for children
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>

// marray - array views
#include "andres/marray.hxx"

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
#include "utils.h"
#include "hdc_storage.h"

//#define DEBUG

using namespace std;
template<typename T> struct identity { typedef T type; };

//this is default global storage
extern HDCStorage* global_storage;

/** Initializes global_storage  -- mainly due to C and Fortran */
void HDC_init();

/** Cleans up global_storage  -- mainly due to C and Fortran */
void HDC_destroy();

/** --------------------- Buffer helper functions ---------------------*/

char* buff_allocate(size_t size);
char* buff_resize(char* buffer, size_t old_size, size_t new_size);
void buff_set_type(char* buffer, TypeID type);
void buff_set_flags(char* buffer, Flags flags);
void buff_set_shape(char* buffer, int ndim, size_t* shape);
void buff_set_shape(char* buffer, size_t shape[HDC_MAX_DIMS]);
void buff_set_header(char* buffer, TypeID type, Flags flags, int ndim, size_t* shape);
TypeID buff_get_type(char* buffer);
Flags buff_get_flags(char* buffer);
size_t* buff_get_shape(char* buffer);
size_t buff_get_size(char* buffer);
int buff_get_ndim(char* buffer);
size_t buff_get_data_size(char* buffer);
char* buff_get_data_ptr(char* buffer);
void buff_dump_header(char* buffer);
bool buff_is_external(char* buffer);
bool buff_is_readonly(char* buffer);
bool buff_is_fortranorder(char* buffer);
void buff_info(char* buffer);

class HDC
{
private:
    map_t* children = nullptr;
    string uuid;
    HDCStorage* storage;
    size_t size = 0;
    size_t shape[HDC_MAX_DIMS];
    TypeID type = EMPTY_ID;
    Flags flags = HDCDefault;
    int ndim = 1;
/* ------------------------------- methods ----------------------------------------- */
    void add_child(vector<string> vs, HDC* n);
    void set_child(vector<string> vs, HDC* n);
    void delete_child(vector<string> vs);
    HDC* get(vector<string> vs);
    HDC* get_slice(vector<string> vs, size_t i);
    bool has_child(vector<string> vs);

public:
    /** Creates empty HDC with specified buffer size */
    HDC(size_t byte_size) {
        if (global_storage == nullptr) {
            HDC_init();
            atexit(HDC_destroy);
        }
        size_t buffer_size = HDC_DATA_POS + byte_size;
        char* data = buff_allocate(buffer_size);
        uuid = generate_uuid_str();
        storage = global_storage;
        storage->set(uuid,data,buffer_size);
        shape[0] = byte_size;
        for(int i=1;i<HDC_MAX_DIMS;i++) shape[i] = 0;
        type = EMPTY_ID;
        flags = HDCDefault;
        size = buffer_size;
    }

    /** Default constructor. Creates empty HDC */
    HDC(): HDC(0) {};

    /** Creates empty HDC with specified type and shape */
    HDC(int _ndim, size_t* _shape, TypeID _type,Flags _flags = HDCDefault) {
        if (ndim >= HDC_MAX_DIMS) {
            cerr << "Unsupported number of dimensions: " << ndim << endl;
            exit(-2);
        }
        size_t elem_size = 1;
        memset(shape,0,sizeof(shape[0])*HDC_MAX_DIMS);
        for (int i = 0; i < ndim; i++) {
            shape[i] = _shape[i];
            elem_size *= _shape[i];
        }
        type = _type;
        flags = _flags;
        ndim = _ndim;
        char* buffer = buff_allocate(elem_size * hdc_sizeof(_type) + HDC_DATA_POS);
        buff_set_header(buffer, type, flags, ndim, shape);
        uuid = generate_uuid_str();
        storage = global_storage;
        storage->set(uuid,buffer,elem_size * hdc_sizeof(_type) + HDC_DATA_POS);
    }

    // TODO: More constructors
    
    /** Destructor */
    ~HDC() {
        storage->remove(uuid);
    }
    size_t get_datasize() {
        //return buff_get_data_size(storage->get(uuid));
        return size + HDC_DATA_POS;
    }
    size_t get_size() {
        //return buff_get_size(storage->get(uuid));
        //return buff_get_size(storage->get_size(uuid));
        return size;
    }
    /** Returns type of current node. */
    TypeID get_type() {
        //return buff_get_type(storage->get(uuid));
        return type;
    }
    Flags get_flags() {
        //return buff_get_flags(storage->get(uuid));
        return flags;
    }
    template<typename T>
    T* get_data() {
        return reinterpret_cast<T*>(buff_get_data_ptr(storage->get(uuid)));
    }
    template<typename T>
    void set_data(int _ndim, size_t* _shape, T* _data, Flags _flags = HDCDefault) {
        if (storage->has(uuid)) storage->remove(uuid);
        type = to_typeid(_data[0]);
        size = hdc_sizeof(type);
        ndim = _ndim;
        memset(shape,0,HDC_MAX_DIMS*sizeof(size_t));
        for (int i = 0; i < _ndim; i++) {
            size *= _shape[i];
            shape[i] = _shape[i];
        }
        size_t buffer_size = size + HDC_DATA_POS;
        char* buffer = buff_allocate(buffer_size);
        buff_set_header(buffer,to_typeid(_data[0]),_flags,_ndim,_shape);
        memcpy(buffer+HDC_DATA_POS,_data,size);
        storage->set(uuid,buffer,buffer_size);
        return;
    }
    template<typename T>
    void set_data(string path, int _ndim, size_t* _shape, T* _data, Flags _flags = HDCDefault) {
       if(!has_child(path)) add_child(path, new HDC()); // TODO: add contructor for this!!
       get(path)->set_data(_ndim, _shape, _data, _flags);
    }
    
    /** Sets scalar data to given node. */
    template <typename T>
    void set_data_scalar(T data) {
        type = to_typeid(data);
        size = sizeof(T);
        shape[0] = 0;
        ndim = 0;
        for (int i = 1; i<HDC_MAX_DIMS; i++) shape[i] = 0;
        char* buffer = buff_allocate(size + HDC_DATA_POS);
        buff_set_header(buffer,type,HDCDefault,1,shape);
        memcpy(buffer+HDC_DATA_POS,&data,sizeof(T));
        storage->set(uuid,buffer,size + HDC_DATA_POS);
    }

    void set_data(string str) {
        size = str.length()+1;
        type = STRING_ID;
        ndim = 1;
        memset(shape,0,HDC_MAX_DIMS*sizeof(size_t));
        shape[0] = size;
        size_t buffer_size = size + HDC_DATA_POS;
        char* buffer = buff_allocate(buffer_size);
        buff_set_header(buffer,type,flags,ndim,shape);
        memcpy(buffer+HDC_DATA_POS,str.c_str(),size);
        storage->set(uuid,buffer,buffer_size);
    };
    
    void set_data(string path, string str) {
        if(!has_child(path)) add_child(path, new HDC()); // TODO: add contructor for this!!
        get(path)->set_data(str);
    }
    
    /** Returns number of dimensions of current node. */
    int get_ndim() {
        //return buff_get_ndim(storage->get(uuid));
        return ndim;
    }
    /** Returns shape of current node. */
    size_t* get_shape() {
        return shape;
    }
    bool is_external() {
        return (flags & HDCExternal) != 0;
    }
    bool is_readonly() {
        return (flags & HDCReadOnly) != 0;
    }
    bool is_fortranorder() {
        return (flags & HDCFortranOrder) != 0;
    }
    void info() {
        printf("Size:\t\t%d\n",size);
        printf("NDim:\t\t%d\n",get_ndim());
        printf("Shape:\t\t"); for (int i=0;i<HDC_MAX_DIMS;i++) printf("%d,",get_shape()[i]);
        printf("\n");
        printf("Data Size:\t\t%d\n",get_datasize());
        printf("External:\t\t%d\n",is_external());
        printf("ReadOnly:\t\t%d\n",is_readonly());
        printf("FortranOrder:\t%d\n",is_fortranorder());
        return;
    }

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
    
    
//     /** Sets data to node on given path from vector<T> data. This function is primarily designed for interoperability with Python */
//     template <typename T> void set_data(string path, vector<T> data) 
//     {
//         if (!has_child(path)) {
//             add_child(path, new HDC());
//             #ifdef DEBUG
//             cout << "\"" << path << "\" not found, adding..." << endl;
//             #endif
//         }
//         get(path)->set_data<T>(data);
//         return;
//     };
// 
//     /** Sets array data to node on given path. */
//     template <typename T> void set_data(string path, int8_t ndim, const long int* shape, void* data, bool copy=true) 
//     {
//         if (!has_child(path)) {
//             add_child(path, new HDC());
//             #ifdef DEBUG
//             cout << "not found, adding..." << endl;
//             #endif
//         }
//         get(path)->set_data<T>(ndim,shape,data,copy=copy);
//         return;
//     };
// 
//     /** Sets scalar data to node on given path. */
//     template <typename T> void set_data(string path, T data) 
//     {
//         if (!has_child(path)) {
//             add_child(path, new HDC());
//             #ifdef DEBUG
//             cout << "not found, adding..." << endl;
//             #endif
//         }
//         get(path)->set_data(data);
//         return;
//     };
    /** Sets HDC_LIST from std::deque<HDC*> data.*/
    void set_list(deque<HDC*>* list); 
    /** Performs deep copy of current node if recursively = 1. Performs shallow copy otherwise. */
    void resize(HDC* h, int recursively = 0); 
    /** Returns copy of current object. */
    HDC* copy(int copy_arrays = 0); 
    /** Inserts node to i-th slice of current node. */
    void insert_slice(size_t i, HDC* h);
    /** Sets node to i-th slice of current node. */
    void set_slice(size_t i, HDC* h);
    /** Appends given node as next available slice (similar to push_back() method seen in C++ STL containers).*/
    void append_slice(HDC* h); 
    /** Sets HDC type of current node. */
    void set_type(TypeID _type);
    /** Returns true if node is empty. */
    bool is_empty(); 
    /** Returns number of dimensions of node under path. */
    int8_t get_ndim(string path); 
    /** Returns shape of node under path. */
    long int* get_shape(string path); 
    /** Returns pointer to data of this node. */
    template<typename T> T as()
    {
        if (children != nullptr) {
            cout << "This node is not terminal" << endl;
            return reinterpret_cast<T>(0);
        }
        #ifdef DEBUG
        cout << "From as:" << data << endl;
        #endif
        if (!storage->has(uuid)) {
            printf("Not found: %s\n",uuid.c_str());
            exit(-3);
        }
        return reinterpret_cast<T>(buff_get_data_ptr(storage->get(uuid)));
    }
    /** Returns string. Needs to have separate function */
    std::string as_string() {
//         if (type == LIST_ID || type == STRUCT_ID) {
//             cout << "as_string is not implemented yet for this type: " << type << endl;
//         }
        #ifdef DEBUG
        cout << "From as_string: \n" << buff_dump_header(storage->get(uuid));<< endl;
        #endif
        string str(buff_get_data_ptr(storage->get(uuid)));
        return str;
    }
    
    /** Returns string of node under given path. Needs to have separate function */
    std::string as_string(string path)
    {
        #ifdef DEBUG
        cout << "as<T>(" << path << ")" << endl;
        #endif
        return get(path)->as_string();
    }
    
    /** Returns pointer to data of node under given path. */
    template<typename T> T as(string path)
    {
        #ifdef DEBUG
        cout << "as<T>(" << path << ")" << endl;
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
};
HDC* from_json(const string& filename);

#endif // HDC_HPP
