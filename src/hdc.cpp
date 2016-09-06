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
    printf("HDC_init(): HDC storage initialized.\n");
}

/** Cleans up global_storage  -- mainly due to C and Fortran */
void HDC_destroy() {
    delete global_storage;
    printf("HDC_destroy(): HDC storage destroyed.\n");
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
    memcpy(buffer+HDC_SHAPE_POS,shape,ndim*sizeof(size_t));
    memset(buffer+HDC_SHAPE_POS+sizeof(size_t)*ndim,0,HDC_MAX_DIMS*sizeof(size_t));
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
size_t buff_get_elem_size(char* buffer) {
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
    return buff_get_elem_size(buffer) + HDC_DATA_POS;
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
    printf("Size:\t\t%d\n",buff_get_elem_size(buffer));
    printf("NDim:\t\t%d\n",buff_get_ndim(buffer));
    printf("Shape:\t\t"); for (int i=0;i<HDC_MAX_DIMS;i++) printf("%d,",buff_get_shape(buffer)[i]);
    printf("\n");
    printf("Data Size:\t\t%d\n",buff_get_data_size(buffer));
    printf("External:\t\t%d\n",buff_is_external(buffer));
    printf("ReadOnly:\t\t%d\n",buff_is_readonly(buffer));
    printf("FortranOrder:\t%d\n",buff_is_fortranorder(buffer));
    return;
}
char* buff_copy(char* buffer) {
    size_t dsize = buff_get_data_size(buffer);
    char* _new = new char[dsize];
    memcpy(_new,buffer,dsize);
};



//---------------------------- HDC class -----------------------------------

/** Creates empty HDC with specified buffer size */
HDC::HDC(size_t byte_size) {
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
HDC::HDC(): HDC((size_t)0) {};

/** Creates empty HDC with specified type and shape */
HDC::HDC(int _ndim, size_t* _shape, TypeID _type,Flags _flags) {
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

/** Copy contructor */
HDC::HDC(HDC* h) {
    storage = h->storage;
    type = h->type;
    ndim = h->ndim;
    flags = h->flags;
    size = h->size;
    uuid = generate_uuid_str();
    storage->set(uuid,h->get_buffer(),size);
    if (h->children != nullptr) {
        switch (type) {
            case (HDC_STRUCT):
            {
                for (size_t i=0;i<h->children->size();i++) {
                    string key = h->children->get<by_index>()[i].key;
                    HDC* node = h->children->get<by_index>()[i].node;
                    add_child(key,new HDC(node));
                    #ifdef DEBUG
                    cout << key << " copied" << endl;
                    #endif
                }
                break;
            }
            case (HDC_LIST):
            {
                for (size_t i = 0; i < h->children->size(); i++) {
                    HDC* node = h->children->get<by_index>()[i].node;
                    insert_slice(i,new HDC(node));
                }
                break;
            }
            default:
            {
                fprintf(stderr,"copy(): Unsupported type of node: %s\n",get_type_str().c_str());
                exit(-3);
            }
        }
    }
}

/** Copy contructor from pointer */
/*HDC::HDC(HDC* h) {
    storage = h->storage;
    type = h->type;
    ndim = h->ndim;
    flags = h->flags;
    size = h->size;
    uuid = generate_uuid_str();
    storage->set(uuid,h->get_buffer(),size);
}
*/

/** Destructor */
HDC::~HDC() {
    storage->remove(uuid);
}

//---------------------------- Header information ----------------------------------
size_t HDC::get_datasize() {
    //return buff_get_data_size(storage->get(uuid));
    return size + HDC_DATA_POS;
}
size_t HDC::get_size() {
    //return buff_get_size(storage->get(uuid));
    //return buff_get_size(storage->get_size(uuid));
    return size;
}
/** Returns type of current node. */
TypeID HDC::get_type() {
    //return buff_get_type(storage->get(uuid));
    return type;
}
Flags HDC::get_flags() {
    //return buff_get_flags(storage->get(uuid));
    return flags;
}
template<typename T>
T* HDC::get_data() {
    return reinterpret_cast<T*>(buff_get_data_ptr(storage->get(uuid)));
}

bool HDC::is_external() {
    return (flags & HDCExternal) != 0;
}
bool HDC::is_readonly() {
    return (flags & HDCReadOnly) != 0;
}
bool HDC::is_fortranorder() {
    return (flags & HDCFortranOrder) != 0;
}
void HDC::info() {
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

//---------------------------- Tree manipulation -----------------------------------


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
            if (i > children->size()) {
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
        printf(",%zu)\n",i);
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
        children = new map_t();
        //children->reserve(HDC_LIST_INIT_CAPACITY);
    }
    return;
}

void* HDC::as_void_ptr() {
    return (void*)this;
}

void HDC::get_header_info() {
    buff_dump_header(storage->get(uuid));
    buff_info(storage->get(uuid));
    return;
}

void HDC::dump() {
    cout << to_json(0) << endl;
}

void HDC::resize(HDC* h, int recursively)
{
    // TODO: discuss this with Jakub and redo this.
    fprintf(stderr,"resize(): not implemented yet...\n");
    exit(-3);
    return;
}

HDC* HDC::copy(int copy_arrays) {
    #ifdef DEBUG
    cout << "Called copy()" << endl;
    #endif
    HDC* _copy = new HDC();
    _copy->set_type(get_type());
    if (hdc_is_primitive_type(type)) {
        if (copy_arrays == 1) {
            _copy->set_buffer(buff_copy(storage->get(uuid)));
        } else return _copy;
    }
    else {
        switch (type) {
            case (EMPTY_ID):
                // Nothing to be done
                break;
            case (STRUCT_ID):
            {
                for (size_t i=0;i<children->size();i++) {
                    string key = children->get<by_index>()[i].key;
                    #ifdef DEBUG
                    cout << "copy: " << key << endl;
                    #endif
                    HDC* node = children->get<by_index>()[i].node;
                    _copy->add_child(key,node->copy(copy_arrays));
                    #ifdef DEBUG
                    cout << key << " copied" << endl;
                    #endif
                }
                break;
            }
            case (LIST_ID):
            {
                for (size_t i = 0; i < children->size(); i++)
                    _copy->insert_slice(i,children->get<by_index>()[i].node->copy(copy_arrays));
                break;
            }
            case (ERROR_ID):
            {
                _copy->set_buffer(get_buffer());
                break;
            }
            default:
            {
                fprintf(stderr,"copy(): Unsupported type of node: %s\n",get_type_str().c_str());
                exit(-3);
            }
        }
    }
    #ifdef DEBUG
    cout << "copy() Done" << endl;
    #endif
    return _copy;
}
void HDC::set_data_c(int _ndim, size_t* _shape, void* _data, TypeID _type) {
    #ifdef DEBUG
    printf("set_data_c(%d, {%d,%d,%d}, %f, %s)\n",_ndim,_shape[0],_shape[1],_shape[2],((double*)_data)[0],hdc_get_type_str(_type).c_str());
    #endif
    if (storage->has(uuid)) storage->remove(uuid);
    type = _type;
    size = hdc_sizeof(type);
    ndim = _ndim;
    memset(shape,0,HDC_MAX_DIMS*sizeof(size_t));
    for (int i = 0; i < _ndim; i++) {
        size *= _shape[i];
        shape[i] = _shape[i];
    }
    char* buffer = buff_allocate(size+HDC_DATA_POS);
    buff_set_header(buffer,type,flags,ndim,shape);
    memcpy(buffer+HDC_DATA_POS,_data,size);
    storage->set(uuid,buffer,size+HDC_DATA_POS);
    return;
}
void HDC::set_data_c(string path, int _ndim, size_t* _shape, void* _data, TypeID _type) {
    if(!has_child(path)) add_child(path, new HDC()); // TODO: add contructor for this!!
    get(path)->set_data_c(_ndim, _shape, _data, _type);
}

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

string HDC::get_type_str(string path) {
    return get(path)->get_type_str();
}

string HDC::get_datashape_str(string path) {
    return get(path)->get_datashape_str();
}

string HDC::get_datashape_str() {
    // TODO: redo this
    return "get_datashape_str() is not implemented yet.";
}

int HDC::get_ndim() {
    return ndim;
}

size_t* HDC::get_shape() {
    return shape;
}

int HDC::get_ndim(string path) {
    //TODO: make more error-proof - add has check -> make it as function???
    return get(path)->get_ndim();
}

size_t* HDC::get_shape(string path) {
    return get(path)->get_shape();
}

void HDC::set_buffer(char* _buffer) {
    if (storage->has(uuid)) {
        storage->remove(uuid);
    }
    uuid = generate_uuid_str();
    type = buff_get_type(_buffer);
    flags= buff_get_flags(_buffer);
    size = buff_get_data_size(_buffer);
    ndim = buff_get_ndim(_buffer);
    memset(shape,0,HDC_MAX_DIMS*sizeof(size_t));
    memcpy(shape,buff_get_shape(_buffer),sizeof(size_t)*ndim);
    storage->set(uuid,_buffer,size);
    return;
}

size_t HDC::childs_count()
{
    return children->size();
}

char* HDC::get_buffer() {
    return storage->get(uuid);
}
