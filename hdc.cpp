#include "hdc.h"

using namespace std;

//---------------------------- Tree manipulation -----------------------------------

hdc::hdc()
{
    cout << "Creating empty node..." << endl;
    type = HDC_EMPTY;
    data = new vector<dynd::nd::array>;
    children = new unordered_map<string, hdc*>;
    //children = nullptr;
    list_elements = nullptr;
}

hdc::hdc(uint8_t i)
{
    cout << "Creating empty node..." << endl;
    type = HDC_EMPTY;
    data = new vector<dynd::nd::array>;
    children = new unordered_map<string, hdc*>; 
//    children = nullptr;
    list_elements = nullptr;
}

hdc::~hdc()
{
    cout << "Desctructor called..." << endl;
    if (!children->empty()) {
        cout << "Deleting children..." << endl;
        for (auto it = children->begin(); it != children->end(); it++) delete it->second;
        children->clear();
    } else {
        delete children;
        delete data;
        if (list_elements != nullptr) {
            for (size_t i;i < list_elements->size();i++) delete &list_elements[i];
            list_elements->clear();
            delete list_elements;
        }
    }
}

bool hdc::has_child(string path)
{
    return this->has_child(split(path,'/'));
}

bool hdc::has_child(vector<string> vs)
{
    cout << "Searching for: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    
    string first = vs[0];
    vs.erase(vs.begin());
    
    if (this->children->count(first) == 0) return false;
    
    if (vs.empty()) {
        return (bool)this->children->count(first);
    } else {
        return this->children->at(first)->has_child(vs);
    }
    return false; // never goes here
}


void hdc::add_child(vector<string> vs, hdc* n) {
    cout << "Adding node: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    
    if (!(this->type == HDC_EMPTY || this->type == HDC_STRUCT)) {
        cout << "Cannot add child to this node. Data assigned???" << endl;
        return;
    }
    if (this->type == HDC_EMPTY) {
        this->type = HDC_STRUCT;
//        this->children = new unordered_map<string, hdc*>;
    }
    
    string first = vs[0];
    vs.erase(vs.begin());
    cout << "still living" << endl;
    if (vs.empty()) {
        
        if (this->children->count(first) == 0) children->insert({first,n});
        else cout << "Error: child already exists!" << endl;
    } else {
        if (this->children->count(first) == 0) {
            children->insert({first,new hdc()});
        }
        children->at(first)->add_child(vs,n);
    }
        cout << "done" << endl;

    return;
}

void hdc::set_list(vector<hdc*>* list) {
    if (this->type != HDC_EMPTY) {
        cout << "Cannot add list to a non-list node." << endl;
    }
    this->list_elements = list;
    return;
}

void hdc::create_list(size_t n) {
    if (this->type != HDC_EMPTY) {
        cout << "Cannot add list to a non-list node." << endl;
    }
    this->type = HDC_LIST;
    this->list_elements = new vector<hdc*>;
    for (size_t i=0; i<n;i++) list_elements->push_back(new hdc());
    //debuging
    for (size_t i=0; i<n;i++) {
        int8_t* arr = new int8_t[1];
        arr[0] = i;
        long shape[] = {1};
        list_elements->at(i)->set_data<int8_t>(1,(long int*)shape,(void*)arr);    
    }
    return;
}

void hdc::add_child(string path, hdc* n)
{
    cout << "Adding child: " << path << endl;
    add_child(split(path,'/'),n);
    return;
}


void hdc::delete_child(vector<string> vs) {
    
    cout << "Delete node: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    
    string first = vs[0];
    vs.erase(vs.begin());
    
    if (this->children->count(first) == 0) return; // Nothing to do here
    
    if (!vs.empty()) {
        delete this->children->at(first);
        this->children->erase(first);
    } else {
        this->children->at(first)->delete_child(vs);
    }
    return;
}

void hdc::delete_child(string path) {
    this->delete_child(split(path,'/'));
    return;
}


hdc* hdc::get_child(vector<string> vs) {
    cout << "Getting node: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    
    string first = vs[0];
    vs.erase(vs.begin());
    
    if (this->children->count(first)) {
        if (vs.empty()) return this->children->at(first);
        else return this->children->at(first)->get_child(vs);
    } else {
        cout << "Not found" << endl;
        return new hdc();
    }
    
}


hdc* hdc::get_slice(vector<string> vs, size_t i) {
    cout << "Getting slice: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    
    string first = vs[0];
    vs.erase(vs.begin());
    
    if (this->children->count(first)) {
        if (vs.empty()) {
            if (i < 0 || i > this->list_elements->size()) {
                cout << "Error: index out of range!" << endl;
                return new hdc();
            }
            return this->list_elements->at(i);
        }
        else return this->children->at(first)->get_slice(vs,i);
    } else {
        cout << "Not found" << endl;
        return new hdc();
    }
}


hdc* hdc::get_slice(size_t i) {
    cout << "Getting slice: " << i << endl;
    
//     if (i < 0 || i > this->list_elements->size()) {
//         cout << "Error: index out of range!" << endl;
//         return new hdc();
//     }
    return this->list_elements->at(i);
}

hdc* hdc::get_slice(string path, size_t i) {
    return this->get_slice(split(path,'/'),i);
}

hdc* hdc::get_child(string path) {
    return this->get_child(split(path,'/'));
}


void hdc::set_child(vector<string> vs, hdc* n) {
    // not sure now, what should this do....
    cout << "Setting node: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    
    if (!this->has_child(vs)) { // Nothing to set
        cout << "Nothing to set, maybe you want to add..." << endl;
        return;
    }
    
    string first = vs[0];
    vs.erase(vs.begin());

    if (vs.empty()) {
        delete this->children->at(first);
        this->children->at(first) = n;
    } else this->children->at(first)->set_child(vs, n);
    
    return;
}


void hdc::set_child(string path, hdc* n) {
    this->set_child(split(path,'/'), n);
    return;
}


//------------------ Data manipulation -------------------------------

uint8_t hdc::get_type()
{
    return this->type;
}

void hdc::set_type(uint8_t i) {
    // More to be added here later
    this->type = i;
    return;
}

int8_t hdc::get_ndim()
{
    if (this->type == HDC_DYND) return this->data->at(0).get_ndim();
    else return 0;
}

long int* hdc::get_shape()
{
    if (this->type == HDC_DYND) {
            long int* shape = (long int*)malloc(this->data->at(0).get_ndim() * sizeof(long int));
            for (size_t i=0; i < this->data->at(0).get_ndim(); i++) shape[i] = this->data->at(0).get_shape()[i];
            //size_t* shape = &(size_t)(this->data->at(0)->get_shape()[0]);
            return shape;
    }
    else if (this->type == HDC_LIST) {
//        return this->data->size();
        return 0;
    }
    else {
        cout << "get_shape() method is not implemented for this type of node: " << this->type << endl;
        return 0;
    }
}

bool hdc::is_empty()
{
    return (this->type == HDC_EMPTY);
}

// void hdc::set_data_int8(int8_t ndim, const long int* shape, int8_t* data)
// {
//     if (this->children->size()) {
//         cout << "The node has already children set..." << endl;
//         return;
//     }
//     
//     dynd::nd::array arr;
//     
//     dynd::ndt::type dint = dynd::ndt::make_type<int8_t>();
//     arr = dynd::nd::dtyped_empty(ndim,shape,dint);
//     arr->data = (char*) data;
//     cout << arr << endl;
//     return;
// }


//------------------ String manipulation -----------------------------

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

vector<string>& split(const string &s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        if (!item.length()) {
            cerr << "Error: empty string between slashes!" << endl;
            exit(-1);
        }
        elems.push_back(item);
    }
}

