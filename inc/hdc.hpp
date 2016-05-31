#ifndef HDC_HPP
#define HDC_HPP

#include <iostream>
#include <dynd/array.hpp>
#include <dynd/type.hpp>
#include <dynd/json_formatter.hpp>
#include <dynd/json_parser.hpp>
#include <dynd/iterator.hpp>
#include <dynd/string_encodings.hpp>
#include <dynd/types/fixed_string_type.hpp>
#include <dynd/types/string_type.hpp>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <json/json.h>
#include <algorithm>

#include "types.h"


#define HDC_EMPTY 0
#define HDC_STRUCT 1
#define HDC_DYND 2
#define HDC_LIST 3
#define HDC_ERROR 4

using namespace std;
template<typename T> struct identity { typedef T type; };

class HDC
{
public:
    /** Default constructor. Creates empty HDC */
    HDC();
    /** Creates HDC with data from DyND array object. */
    HDC(dynd::nd::array arr);
    /** Desctructor */
    ~HDC();
    /** Adds HDC subtree as child with given path. If neccessary, recursively creates subnodes. */
    void add_child(string path, HDC* n);
    /** Sets HDC subtree to given path. */
    void set_child(string path, HDC* n);
    /** Deletes HDC subtree. */
    void delete_child(string path);
    /** Returns subtree by path. */
    HDC* get_child(string path);
    /** Returns i-th subnode if HDC_LIST is the type. */
    HDC* get_slice(string path, size_t i); 
    /** Returns i-th subnode if HDC_LIST is the type. */
    HDC* get_slice(size_t i); 
    /** Returns true if subtree with given path with exists and false otherwise. */
    bool has_child(string path); 
    /** Sets data to current node from vector<T> data. This function is primarily designed for interoperability with Python */
    template <typename T> void set_data(vector<T> data) 
    {
        if (this->children->size()) {
            cout << "The node has already children set..." << endl;
            return;
        }
        //dynd::nd::array arr;
        //dynd::ndt::type dtype = dynd::ndt::make_type<T>();
        //arr->data = (char*) data;
        dynd::nd::array arr = data;
        cout << arr << endl;
        if (this->data->size()) this->data->clear();
        this->data->push_back(arr);
        //this->data = arr;
        this->type = HDC_DYND;
        return;
    };
    /** Sets data to node on given path from vector<T> data. This function is primarily designed for interoperability with Python */
    template <typename T> void set_data(string path, vector<T> data) 
    {
        if (!this->has_child(path)) {
            this->add_child(path, new HDC());
            cout << "not found, adding..." << endl;
        }
        this->get_child(path)->set_data<T>(data);
        return;
    };
    /** Sets array data to current node. */
    template <typename T> void set_data(int8_t ndim, const long int* shape, void* data) 
    {
        if (this->children->size()) {
            cout << "The node has already children set..." << endl;
            return;
        }
        dynd::nd::array arr;
        dynd::ndt::type dtype = dynd::ndt::make_type<T>();
        arr = dynd::nd::dtyped_empty(ndim,shape,dtype);
        arr->data = (char*) data;
//         arr.assign(data); // New versions of DyND
        cout << arr << endl;
        if (this->data->size()) this->data->clear();
        this->data->push_back(arr);
        //this->data = arr;
        this->type = HDC_DYND;
        return;
    };
    /** Sets array data to node on given path. */
    template <typename T> void set_data(string path, int8_t ndim, const long int* shape, void* data) 
    {
        if (!this->has_child(path)) {
            this->add_child(path, new HDC());
            cout << "not found, adding..." << endl;
        }
        this->get_child(path)->set_data<T>(ndim,shape,data);
        return;
    };
    /** Sets scalar data to current node. */
    template <typename T> void set_data(T data) 
    {
        if (this->children->size()) {
            cout << "The node has already children set..." << endl;
            return;
        }
        dynd::nd::array arr = data;
        cout << arr << endl;
        if (this->data->size()) this->data->clear();
        this->data->push_back(arr);
        this->type = HDC_DYND;
        return;
    };
    /** Sets scalar data to node on given path. */
    template <typename T> void set_data(string path, T data) 
    {
        if (!this->has_child(path)) {
            this->add_child(path, new HDC());
            cout << "not found, adding..." << endl;
        }
        this->get_child(path)->set_data(data);
        return;
    };
    /** Sets DyND object to current node. */
    void set_dynd(dynd::nd::array array); 
    /** Sets HDC_LIST from std::vector<HDC*> data.*/
    void set_list(vector<HDC*>* list); 
    /** Creates list with some data in nodes - just for testing some ideas. */
    void create_list(size_t n=5); 
    /** Performs deep copy of current node if recursively = 1. Performs shallow copy otherwise. */
    void resize(HDC* h, int recursively = 0); 
    /** Returns copy of current object. */
    HDC* copy(int copy_arrays = 0); 
    /** Sets node to i-th slice of current node. */
    void set_slice(size_t i, HDC* h); 
    /** Appends given node as next available slice (similar to push_back() method seen in C++ STL containers).*/
    void append_slice(HDC* h); 
    /** Returns type of current node. */
    uint8_t get_type(); 
    /** Sets HDC type of current node. */
    void set_type(uint8_t i); 
    /** Returns true if node is empty. */
    bool is_empty(); 
    /** Returns number of dimensions of current node. */
    int8_t get_ndim(); 
    /** Shape of array. */
    long int* get_shape(); 
    /** Returns number of dimensions of node under path. */
    int8_t get_ndim(string path); 
    /** Returns shape of node under path. */
    long int* get_shape(string path); 
    
    /** Returns pointer to data of this node. */
    template<typename T> T as()
    {
        // returns data of given type
        if (this->children->size()) {
            cout << "This node is not terminal" << endl;
        }
        #ifdef DEBUG
        cout << "From get:" << this->data->at(0) << endl;
        #endif
        return (T)(this->data->at(0)->data);
    }
    /** Returns pointer to data of node under given path. */
    template<typename T> T as(string path)
    {
        #ifdef DEBUG
        cout << "as<T>(" << path << ")" << endl;
        #endif
        return this->get_child(path)->as<T>();
    }
    /** Returns double. */
    double as_double() 
    {
        return this->as<double*>()[0];
    }
    /** Returns double. */
    double as_double(string path) 
    {
        return this->as<double*>(path)[0];
    }
    /** Returns pointer to self. */
    hdc_t* as_hdc_ptr(); 
    
    /** Serialization to JSON file. */
    void to_json(string filename, int mode = 0);
    /** Serialization to Json::Value object. */
    Json::Value to_json(int mode = 0); 

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
private:
    int8_t type;
    vector<dynd::nd::array>* data; /*!< dynd::array storage*/
    //     dynd::nd::array data;
    vector<HDC*>* list_elements; /*!< dynd::array storage - list type*/
    unordered_map<string, HDC*>* children; /*!< dynd::array storage - struct type*/
    
    void add_child(vector<string> vs, HDC* n);
    void set_child(vector<string> vs, HDC* n);
    void delete_child(vector<string> vs);
    HDC* get_child(vector<string> vs);
    HDC* get_slice(vector<string> vs, size_t i);
    bool has_child(vector<string> vs);
};

vector<string>& split(const string &s, char delim, vector<string>& elems);
vector<string> split(const string &s, char delim);
void replace_all(std::string& str, const std::string& from, const std::string& to);

/** Loads tree from JSON file. */
HDC* from_json(const string& filename); 
/** Loads tree from Json::Value object */
HDC* json_to_hdc(Json::Value* root);
/** Loads tree from Json::Value object */
HDC* json_to_hdc(const Json::Value& root);

int64_t detect_array_type(Json::Value* root);
bool is_all_numeric(Json::Value* root);
bool is_double(Json::Value* root);
bool is_jagged(Json::Value* root);
long* get_shape(Json::Value* root);
int8_t get_ndim(Json::Value* root);

int64_t detect_array_type(const Json::Value& root);
bool is_all_numeric(const Json::Value& root);
bool is_double(const Json::Value& root);
bool is_jagged(const Json::Value& root);
long* get_shape(const Json::Value& root);
int8_t get_ndim(const Json::Value& root);



void hello__();

#endif // HDC_HPP
