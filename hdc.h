#ifndef HDC_H
#define HDC_H

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

using namespace std;
template<typename T> struct identity { typedef T type; };

class hdc
{
public:
    hdc(); // Creates empty HDC
    hdc(uint8_t i);
    ~hdc();
    void add_child(string path, hdc* n); // Add subnode
    void set_child(string path, hdc* n); // Set subnode
    void delete_child(string path); // Deletes subtree
    hdc* get_child(string path); // Get subnode
    hdc* get_slice(string path, size_t i); // Get i-th subnode if HDC_LIST is the type...
    hdc* get_slice(size_t i);
    bool has_child(string path);
    // Data manipulation methods
    template <typename T> void set_data(int8_t ndim, const long int* shape, void* data) {
        if (this->children->size()) {
            cout << "The node has already children set..." << endl;
            return;
        }
        dynd::nd::array arr;
        dynd::ndt::type dtype = dynd::ndt::make_type<T>();
        arr = dynd::nd::dtyped_empty(ndim,shape,dtype);
        arr->data = (char*) data;
        cout << arr << endl;
        if (this->data->size()) this->data->clear();
        this->data->push_back(arr);
        //this->data = arr;
        this->type = HDC_DYND;
        return;
    };
    template <typename T> void set_data(string path, int8_t ndim, const long int* shape, void* data) {
        hdc* t = this->get_child(path);
        t->set_data<T>(ndim,shape,data);
        return;
    };
    
    template <typename T> void set_data(T data) {
        if (this->children->size()) {
            cout << "The node has already children set..." << endl;
            return;
        }
        dynd::nd::array arr = data;
        cout << arr << endl;
        if (this->data->size()) this->data->clear();
        this->data->push_back(arr);
        //this->data = arr;
        this->type = HDC_DYND;
        return;
    };
    template <typename T> void set_data(string path, T data) {
        hdc* t = this->get_child(path);
        t->set_data(data);
        return;
    };
    
    
    //void set_string(string str); not needed
    void set_json(string json);
    void set_list(vector<hdc*>* list);
    void create_list(size_t n=5);
    void resize(hdc* h, int recursively = 0);
    hdc* copy(int copy_arrays = 0);
    void set_slice(size_t i, hdc* h);
    void append_slice(hdc* h);
    uint8_t get_type();
    void set_type(uint8_t i);
    bool is_empty();
    int8_t get_ndim(); // obtained from dynd::array
    long int* get_shape(); // obtained from dynd::array
    int8_t get_ndim(string path); // obtained from dynd::array
    long int* get_shape(string path); // obtained from dynd::array
    template<typename T> T as() {
        // returns data of given type
        if (this->children->size()) {
            cout << "This node is not terminal" << endl;
        }
        cout << "From get:" << this->data->at(0) << endl;
        
        return (T)(this->data->at(0)->data);
        //return (T)(this->data.data());
    }
    
    template<typename T> T as(string path) {
        // returns data of given type
        hdc* t = this->get_child(path);
        return t->as<T>();
    }
    
    // Serialization
    void to_json(string filename, int mode = 0);
    Json::Value to_json(int mode = 0);
    
private:
    int8_t type;
    vector<dynd::nd::array>* data;
    //     dynd::nd::array data;
    vector<hdc*>* list_elements;
    unordered_map<string, hdc*>* children;
    
    void add_child(vector<string> vs, hdc* n);
    void set_child(vector<string> vs, hdc* n);
    void delete_child(vector<string> vs);
    hdc* get_child(vector<string> vs);
    hdc* get_slice(vector<string> vs, size_t i);
    bool has_child(vector<string> vs);
    
};

vector<string>& split(const string &s, char delim, vector<string>& elems);
vector<string> split(const string &s, char delim);
void replace_all(std::string& str, const std::string& from, const std::string& to);

hdc* from_json(string filename);
hdc* json_to_hdc(Json::Value* root);
// hdc* json_to_hdc(Json::Value& root);

int64_t detect_array_type(Json::Value* root);
bool is_all_numeric(Json::Value* root);
bool is_double(Json::Value* root);
bool is_jagged(Json::Value* root);

#endif // HDC_H
