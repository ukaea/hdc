#ifndef HDC_H
#define HDC_H

#include <iostream>
#include <dynd/array.hpp>
#include <dynd/type.hpp>
#include <dynd/json_formatter.hpp>
#include <dynd/json_parser.hpp>
#include <dynd/iterator.hpp>
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
               
//         cout << "Ndim:  " << (int)ndim << endl;
//         cout << "shape: " << (long)shape[0] << endl;
//         cout << "data:  " << (int)((int8_t*)data)[0] << (int)((int8_t*)data)[1] << (int)((int8_t*)data)[2] << (int)((int8_t*)data)[3] <<endl;
        
        dynd::nd::array arr;
        
        dynd::ndt::type dtype = dynd::ndt::make_type<T>();
        arr = dynd::nd::dtyped_empty(ndim,shape,dtype);
        arr->data = (char*) data;
        cout << arr << endl;
        
        this->data->push_back(arr);
        //this->data = arr;
        this->type = HDC_DYND;
        return;
    }
    void set_list(vector<hdc*>* list);
    void create_list(size_t n=5);
    void resize();
    uint8_t get_type();
    void set_type(uint8_t i);
    bool is_empty();
    int8_t get_ndim(); // obtained from dynd::array
    long int* get_shape(); // obtained from dynd::array
    template<typename T> T as() {
        // returns data of given type
        if (this->children->size()) {
            cout << "This node is not terminal" << endl;
        }
        cout << "From get:" << this->data->at(0) << endl;
        
        return (T)(this->data->at(0)->data);
        //return (T)(this->data.data());
    }
    // Serialization
    void to_json(string filename, int mode = 0);
    Json::Value to_json(int mode = 0);
    hdc* from_json(string filename);
    
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


#endif // HDC_H
