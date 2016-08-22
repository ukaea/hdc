#include "hdc.hpp"
#include <fstream>
//#define DEBUG
struct hdc_t {
    void* obj;
};


using namespace std;

HDCStorage* global_storage = nullptr;

/** Initializes global_storage  -- mainly due to C and Fortran */
void HDC_init() {
    global_storage = new HDCStorage("libUMapPlugin.so","");
    //global_storage = new HDCStorage("libMDBMPlugin.so","{\"filename\": \"/tmp/db1.mdbm\"}");
    printf("HDC storage initialized.");
}

/** Cleans up global_storage  -- mainly due to C and Fortran */
void HDC_destroy() {
    delete global_storage;
    printf("HDC storage destroyed.");
}

/** --------------------- Buffer helper functions ---------------------*/

char* buff_allocate(size_t size) {
    return new char[size];
}
char* buff_resize(char* buffer, size_t old_size, size_t new_size) {
    char* new_buffer = buff_allocate(new_size);
    memcpy(new_buffer,buffer,min(old_size,new_size));
    delete[] buffer;
    return buffer;
}
void buff_set_type(char* buffer, TypeID type) {
    memcpy(buffer+HDC_TYPE_POS,&type,sizeof(TypeID));
}
void buff_set_flags(char* buffer, Flags flags) {
    memcpy(buffer+HDC_FLAGS_POS,&flags,sizeof(Flags));
}
void buff_set_shape(char* buffer, int ndim, size_t* shape) {
    for (int i=0;i<ndim;i++)
        buffer[HDC_SHAPE_POS+i*sizeof(size_t)] = shape[i];
    if (ndim < HDC_MAX_DIMS)
        for (int i=ndim+1;i<HDC_MAX_DIMS;i++)
            buffer[HDC_SHAPE_POS+i*sizeof(size_t)] = 0;
}
void buff_set_shape(char* buffer, size_t shape[HDC_MAX_DIMS]) {
    for (int i=0;i<HDC_MAX_DIMS;i++)
        buffer[HDC_SHAPE_POS+i*sizeof(size_t)] = shape[i];
}
void buff_set_header(char* buffer, TypeID type, Flags flags, int ndim, size_t* shape) {
    buff_set_type(buffer,type);
    buff_set_flags(buffer,flags);
    buff_set_shape(buffer,ndim,shape);
}
TypeID buff_get_type(char* buffer) {
    return (TypeID)((unsigned long)(buffer[HDC_TYPE_POS]));
}
Flags buff_get_flags(char* buffer) {
    return static_cast<Flags>(buffer[HDC_FLAGS_POS]);
}
size_t* buff_get_shape(char* buffer) {
    return reinterpret_cast<size_t*>(buffer+HDC_SHAPE_POS);
}
size_t buff_get_size(char* buffer) {
    size_t size = hdc_sizeof(buff_get_type(buffer));
    if (size > 0) {
        for (int i = 0;i<HDC_MAX_DIMS;i++) {
            if (buffer[HDC_SHAPE_POS+i*sizeof(size_t)] == 0) break;
            size *= buffer[HDC_SHAPE_POS+i*sizeof(size_t)];
        }
    }
    return size;
}
int buff_get_ndim(char* buffer) {
    size_t size = hdc_sizeof(buff_get_type(buffer));
    if (buff_get_type(buffer) == STRUCT_ID) return 1;
    if (buff_get_type(buffer) == ERROR_ID) return 1;
    if (buff_get_type(buffer) == EMPTY_ID) return 1;
    if (buff_get_type(buffer) == STRING_ID) return 1;
    int ndim = 0;
    if (size > 0) {
        for (int i = 0;i<HDC_MAX_DIMS;i++) {
            if (buffer[HDC_SHAPE_POS+i*sizeof(size_t)] == 0) break;
            ndim++;
        }
    }
    return ndim;
}
size_t buff_get_data_size(char* buffer) {
    return buff_get_size(buffer) + HDC_DATA_POS;
}
char* buff_get_data_ptr(char* buffer) {
    return buffer + HDC_DATA_POS;
}
void buff_dump_header(char* buffer) {
    for (size_t i = 0; i < HDC_DATA_POS; i++) {
        printf("%02X", buffer[i]);
        if (i % 8 == 7) printf(" ");
    }
    printf("\n");
}
bool buff_is_external(char* buffer) {
    return (static_cast<unsigned long>(buffer[HDC_FLAGS_POS]) & HDCExternal) != 0;
}
bool buff_is_readonly(char* buffer) {
    return (static_cast<unsigned long>(buffer[HDC_FLAGS_POS]) & HDCReadOnly) != 0;
}
bool buff_is_fortranorder(char* buffer) {
    return (static_cast<unsigned long>(buffer[HDC_FLAGS_POS]) & HDCFortranOrder) != 0;
}
void buff_info(char* buffer) {
    printf("Type:\t\t%d\n",buff_get_type(buffer));
    printf("Size:\t\t%d\n",buff_get_size(buffer));
    printf("NDim:\t\t%d\n",buff_get_ndim(buffer));
    printf("Shape:\t\t"); for (int i=0;i<HDC_MAX_DIMS;i++) printf("%d,",buff_get_shape(buffer)[i]);
    printf("\n");
    printf("Data Size:\t\t%d\n",buff_get_data_size(buffer));
    printf("External:\t\t%d\n",buff_is_external(buffer));
    printf("ReadOnly:\t\t%d\n",buff_is_readonly(buffer));
    printf("FortranOrder:\t%d\n",buff_is_fortranorder(buffer));
    return;
}



//---------------------------- Tree manipulation -----------------------------------
// HDC::HDC()
// {
//     #ifdef DEBUG
//     cout << "Creating empty node..." << endl;
//     #endif
//     type = HDC_EMPTY;
//     data = dynd::nd::array();
//     children = new unordered_map<string, HDC*>;
//     //children = nullptr;
//     children = nullptr;
// }
// 
// HDC::HDC(dynd::nd::array&& arr) {
//     #ifdef DEBUG
//     cout << "Creating DyND node..." << endl;
//     #endif
//     type = HDC_DYND;
//     data = std::move(arr);
//     children = new unordered_map<string, HDC*>;
//     children = nullptr;
// }
// 
// HDC::~HDC()
// {
//     #ifdef DEBUG
//     cout << "Destructor called..." << endl;
//     #endif
//     if (!children->empty()) {
//         #ifdef DEBUG
//         cout << "Deleting children..." << endl;
//         #endif
//         for (auto it = children->begin(); it != children->end(); it++) {
//             delete it->second;
//             #ifdef DEBUG
//             cout << "Remove: " << it->first << endl;
//             #endif
//         }
//         children->clear();
//     } else {
//         delete children;
//         if (children != nullptr) {
//             children->erase(remove_if(children->begin(),children->end(),deleteAll));
//             delete children;
//         }
//     }
// }
// 
bool HDC::has_child(string path)
{
    return has_child(split(path,'/'));
}

bool HDC::has_child(vector<string> vs)
{
    #ifdef DEBUG
    printf("has_child(");
    for (long i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(")\n");
    #endif
    if(vs.empty()) return false; //TODO: re-do this!!!
    string first = vs[0];
    vs.erase(vs.begin());
    if (children == nullptr) return false;
    if (children->count(first) == 0) return false;

    if (vs.empty()) {
        return (bool)children->count(first);
    } else {
        return static_cast<record>(*(children->find(first))).node->has_child(vs);
    }
    return false; // never goes here
}

void HDC::add_child(vector<string> vs, HDC* n) {
    #ifdef DEBUG
    printf("set_child(");
    for (long i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(")\n");
    #endif
    if (!(type == EMPTY_ID || type == STRUCT_ID)) {
        cout << "Cannot add child to this node. Data assigned???" << endl;
        return;
    }
    if (type == EMPTY_ID) {
        set_type(STRUCT_ID);
    }
    string first = vs[0];
    vs.erase(vs.begin());
    if (vs.empty()) {
        if (children->count(first) == 0) children->insert({first,n});
        else cout << "Error: child already exists!" << endl;
    } else {
        if (children->count(first) == 0) {
            children->insert({first,new HDC()});
        }
        static_cast<record>(*(children->find(first))).node->add_child(vs,n);
    }
    return;
}

vector<string> HDC::keys() {
    vector<string> k;
    if (children == nullptr) {
        return k;
    }
    k.reserve(children->size());
    for (map_t::iterator it = children->begin(); it != children->end(); ++it) {
        k.push_back(it->key);
    }
    return k;
}

void HDC::add_child(string path, HDC* n)
{
    #ifdef DEBUG
    printf("add_child(%s)\n",path.c_str());
    #endif
    add_child(split(path,'/'),n);
    return;
}

void HDC::delete_child(vector<string> vs) {

    #ifdef DEBUG
    printf("delete_child(");
    for (long i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(")\n");
    #endif
    if (!has_child(vs) || vs.empty())  {
        return;
    }
    string first = vs[0];
    vs.erase(vs.begin());
    if (vs.empty()) {
        children->erase(first);
    } else {
        static_cast<record>(*(children->find(first))).node->delete_child(vs);
    }
    // set type back to empty if the only child was deleted.
    if (children->empty()) set_type(EMPTY_ID);
    return;
}

void HDC::delete_child(string path) {
    delete_child(split(path,'/'));
    return;
}

HDC* HDC::get(vector<string> vs) {
    #ifdef DEBUG
    printf("get(");
    for (long i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(")\n");
    #endif
    string first = vs[0];
    vs.erase(vs.begin());

    if (children->count(first)) {
        if (vs.empty()) return static_cast<record>(*(children->find(first))).node;
        else return static_cast<record>(*(children->find(first))).node->get(vs);
    } else {
        cout << "Not found" << endl;
        return new HDC();
    }
}

HDC* HDC::get_slice(vector<string> vs, size_t i) {
    #ifdef DEBUG
    printf("get_slice(");
    for (long i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(",%d)\n",i);
    #endif
    string first = vs[0];
    vs.erase(vs.begin());
    if (children->count(first)) {
        if (vs.empty()) {
            if (type != HDC_LIST) return this;
            if (i < 0 || i > children->size()) {
                cout << "Error: index out of range!" << endl;
                return new HDC();
            }
            return children->get<by_index>()[i].node;
        }
        else return static_cast<record>(*(children->find(first))).node->get_slice(vs,i);
    } else {
        printf("Not found: get_slice(");
        for (long i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
        printf("%s",vs[vs.size()-1].c_str());
        printf(",%d)\n",i);
        return new HDC();
    }
}


HDC* HDC::get_slice(size_t i) {
    #ifdef DEBUG
    printf("get_slice(%d)\n",i);
    #endif
    if (children == nullptr) return this;
    if (type == LIST_ID) return children->get<by_index>()[i].node;
    else return this; // return this if not list
}

HDC* HDC::get_slice(string path, size_t i) {
    return get_slice(split(path,'/'),i);
}

HDC* HDC::get(string path) {
    return get(split(path,'/'));
}

void HDC::set_child(vector<string> vs, HDC* n) {
    #ifdef DEBUG
    printf("set_child(");
    for (long i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(")\n");
    #endif
    if (!has_child(vs)) { // Nothing to set
        cout << "Nothing to set, maybe you want to add..." << endl;
        return;
    }
    string first = vs[0];
    vs.erase(vs.begin());

    if (vs.empty()) {
        // check whether children container exists
        if (children == nullptr) children = new map_t;
        // check whether given path exists
        if (children->count(first)) {
            children->modify(children->find(first), change_node(n));
        } else {
            children->insert({first,n});
        }
    } else static_cast<record>(*(children->find(first))).node->set_child(vs, n);
    return;
}


void HDC::set_child(string path, HDC* n) {
    set_child(split(path,'/'), n);
    return;
}

//------------------ Data manipulation -------------------------------


void HDC::set_type(TypeID _type) {
    // More to be added here later
    type = _type;
    if ((_type == LIST_ID || _type == STRUCT_ID) && children == nullptr) {
        children = new map_t;
        children->reserve(HDC_LIST_INIT_CAPACITY);
    }
    return;
}

void* HDC::as_void_ptr() {
    return (void*)this;
}

void HDC::dump() {
    //cout << to_json(0);
    buff_dump_header(storage->get(uuid));
    buff_info(storage->get(uuid));
    return;
}

// 
// 
// void HDC::resize(HDC* h, int recursively)
// {
//     if (type == HDC_DYND) {
//         if (!recursively) cout << "Operation is yet not supported on the DYND node." << endl;
//         return;
//     }
//     else if (type == HDC_LIST) {
//         if (!recursively) cout << "Operation is yet not supported on the list node." << endl;
//         return;
//     }
//     else if (type == HDC_STRUCT) {
//         for (auto it = h->children->begin(); it != h->children->end(); ++it) {
//             if (!has_child(it->first)) add_child(it->first,it->second->copy(0));
//         }
//         return;
//     }
//     return;
// }
// 
// HDC* HDC::copy(int copy_arrays)
// {
//     #ifdef DEBUG
//     cout << "Called copy()" << endl;
//     #endif
//     HDC* copy = new HDC();
//     copy->set_type(get_type());
//     #ifdef DEBUG
//     cout << (int)(get_type()) << endl;
//     #endif
//     if (type == HDC_STRUCT) {
//         for (auto it = children->begin(); it != children->end(); it++) {
//             copy->add_child(it->first,it->second->copy(copy_arrays));
//             #ifdef DEBUG
//             cout << it->first << " copied" << endl;
//             #endif
//         }
//     } else if (type == HDC_LIST) {
//         for (size_t i = 0; i < children->size(); i++)
//             copy->insert_slice(i,children->at(i)->copy(copy_arrays));
//     }
//     else if (type == HDC_DYND) {
//         if (copy_arrays == 1) {
//             //dynd::nd::empty(<data.get_type()>);
//             //long size = data.size();
//             //char *
//             copy->data = data;
//             #ifdef DEBUG
//             cout << "copy: " << copy->data << endl;
//             #endif
//         } else set_type(HDC_EMPTY);
//     }
//     #ifdef DEBUG
//     cout << "copy() Done" << endl;
//     #endif
//     return copy;
// }

void HDC::insert_slice(size_t i, HDC* h)
{
    #ifdef DEBUG
    printf("insert_slice(%d)\n",i);
    #endif
    map_t::nth_index<1>::type& ri=children->get<1>();
    switch (type) {
        case EMPTY_ID:
            set_type(LIST_ID);
        case LIST_ID:
            ri.insert(ri.begin()+i,{"_",h});
            break;
        default:
            cout << "Error in insert_slice(): Wrong type to call insert_slice." << endl;
            break;
    }
    return;
}

void HDC::set_slice(size_t i, HDC* h)
{
    #ifdef DEBUG
    printf("set_slice(%d)\n",i);
    #endif
    switch (type) {
        case LIST_ID:
            if (i >= children->size()) {
                cout << "Error in set_slice: Index " << i << " >= list size." << endl;
                return;
            }
            children->modify(children->iterator_to(children->get<by_index>()[i]), change_node(h));
            break;
            //TODO: Add support for HDC_STRUCT here? How?
        default:
            cout << "Error in insert_slice(): Wrong type to call insert_slice." << endl;
            break;
    }
    return;
}


void HDC::append_slice(HDC* h) {
    if (type == EMPTY_ID) {
        set_type(LIST_ID);
    }
    if (type == LIST_ID) {
        insert_slice(shape[0]++,h);
    } else {
        cout << "Error: append_slice not supported for HDC_STRUCT" << endl;
        exit(-1);
    }
    return;
}

hdc_t* HDC::as_hdc_ptr() {
    hdc_t* wrap = new struct hdc_t;
    wrap->obj = (void*) this;
    return wrap;
}

string HDC::get_type_str() {
    return hdc_get_type_str(type);
}

// string HDC::get_type_str(string path) {
//     return get(path)->get_type_str();
// }
// 
// string HDC::get_datashape_str(string path) {
//     return get(path)->get_datashape_str();
// }
// string HDC::get_datashape_str() {
//     string type_str;
//     if (type == HDC_EMPTY) type_str = "null";
//     else if (type == HDC_LIST || type == HDC_STRUCT) type_str = "hdc";
//     else {
//         dynd::ndt::type dt;
//         dt = data.get_type();
//         type_str = dt.str();
//     }
//     return type_str;
// }