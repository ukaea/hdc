#ifndef HDC_H
#define HDC_H

#include <iostream>
#include <dynd/array.hpp>
#include <dynd/type.hpp>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cstdlib>

#include "types.h"


#define EMPTY 0
#define NONTERMINAL 1
#define DYND 2
#define DYND_LIST 3

using namespace std;
template<typename T> struct identity { typedef T type; };

class hdc
{
public:
    hdc();
    ~hdc();
    void add_child(string path, hdc* n); // Add subnode
    void set_child(string path, hdc* n); // Set subnode
    void delete_child(string path); // Deletes subtree
    hdc* get_child(string path); // Get subnode
    bool has_child(string path);
    // Data manipulation methods
//     void set_data_int8(int8_t ndim, const long int* shape, int8_t* data); // this should be the put method
    template <typename T> void set_data(int8_t ndim, const long int* shape, void* data) {
        if (this->children->size()) {
            cout << "The node has already children set..." << endl;
            return;
        }
               
        cout << "Ndim:  " << (int)ndim << endl;
        cout << "shape: " << (long)shape[0] << endl;
        cout << "data:  " << (int)((int8_t*)data)[0] << (int)((int8_t*)data)[1] << (int)((int8_t*)data)[2] << (int)((int8_t*)data)[3] <<endl;
        
        
        
        dynd::nd::array arr;
        
        dynd::ndt::type dtype = dynd::ndt::make_type<T>();
        arr = dynd::nd::dtyped_empty(ndim,shape,dtype);
        arr->data = (char*) data;
        cout << arr << endl;
        
        this->data->push_back(arr);
        this->type = DYND;
        return;
    }
    uint8_t get_type();
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
    }
private:
    int8_t type;
    vector<dynd::nd::array>* data;
    unordered_map<string, hdc*>* children;
    
    void add_child(vector<string> vs, hdc* n);
    void set_child(vector<string> vs, hdc* n);
    void delete_child(vector<string> vs);
    hdc* get_child(vector<string> vs);
    bool has_child(vector<string> vs);
    
};

vector<string>& split(const string &s, char delim, vector<string>& elems);
vector<string> split(const string &s, char delim);



#endif // HDC_H
