#include "hdc.hpp"
#include <fstream>
#include <memory>
//#define DEBUG
struct hdc_t {
    void* obj;
};


using namespace std;

HDCStorage* global_storage = nullptr;

/** Initializes global_storage  -- mainly due to C and Fortran */
void HDC_init(string pluginFileName, string pluginSettingsFileName) {

    // First , try to load the file under filename, if not exists try some paths
    string pluginPath = "";
    
    if (boost::filesystem::exists(pluginFileName)) {
        // OK, load this
        pluginPath = boost::filesystem::absolute(pluginFileName).string();
    } else {
        // Never mind, try some default paths -- Now I don't know how do this better...
        boost::filesystem::path p(pluginFileName);
        string strippedName = p.filename().string();
        vector<string> pluginSearchPath;
        pluginSearchPath.push_back("./");
        pluginSearchPath.push_back("./plugins");
        pluginSearchPath.push_back(".config/hdc/plugins");
        pluginSearchPath.push_back("/usr/local/lib");
        pluginSearchPath.push_back("/usr/lib");
        pluginSearchPath.push_back("/usr/local/lib64");
        pluginSearchPath.push_back("/usr/lib64");
        pluginSearchPath.push_back("/usr/local/lib/hdc");
        pluginSearchPath.push_back("/usr/lib/hdc");
        pluginSearchPath.push_back("/usr/local/lib64/hdc");
        pluginSearchPath.push_back("/usr/lib64/hdc");
        // Search all paths and stop if found
        for (auto path : pluginSearchPath) {
            string tmp = path+'/'+strippedName;
            if (boost::filesystem::exists(tmp)) {
                cout << "Plugin found: " << tmp << endl;
                pluginPath = tmp;
                break;
            }
        }
    }
    // If selected, check whether file settings file exists
    if (pluginSettingsFileName.size() != 0) {
        if (!boost::filesystem::exists(pluginSettingsFileName)) {
            cerr << "Settings file set, but does not exist: " << pluginSettingsFileName << endl;
            exit(-1);
        }
    }

    global_storage = new HDCStorage(pluginPath,pluginSettingsFileName);
    printf("HDC_init(): HDC storage initialized.\n");
}

/** Cleans up global_storage  -- mainly due to C and Fortran */
void HDC_destroy() {
    delete global_storage;
    printf("HDC_destroy(): HDC storage destroyed.\n");
}

//---------------------------- HDC class -----------------------------------

/** Creates empty HDC with specified buffer size */
HDC::HDC(size_t _data_size) {
    // fill some data
    memset(&header,0,sizeof(header_t));
    header.buffer_size = _data_size + sizeof(header_t);
    header.data_size = _data_size;
    
    if (global_storage == nullptr) {
//        HDC_init("./plugins/libMDBMPlugin.so","./plugins/settings.txt");
        HDC_init();
        atexit(HDC_destroy);
    }
    
    // Start by creating segment
    char* buffer = new char[header.buffer_size];

    // copy header there -- we need that, hopefully it will be optimized out
    memcpy(buffer,&header,sizeof(header_t));
    
    //Store to some storage
    uuid = generate_uuid_str();
    cout << uuid << "\n";
    storage = global_storage;
    storage->set(uuid,buffer,header.buffer_size);
}

/** Default constructor. Creates empty HDC */
HDC::HDC(): HDC((size_t)4096+256) {};

/** Creates empty HDC with specified type and shape */
HDC::HDC(int _ndim, size_t* _shape, size_t _type,Flags _flags) {
    std::cout << "Fixme\n";
    exit(7);
// //     if (ndim >= HDC_MAX_DIMS) {
// //         cerr << "Unsupported number of dimensions: " << ndim << endl;
// //         exit(-2);
// //     }
// //     size_t elem_size = 1;
// //     memset(shape,0,sizeof(shape[0])*HDC_MAX_DIMS);
// //     for (int i = 0; i < ndim; i++) {
// //         shape[i] = _shape[i];
// //         elem_size *= _shape[i];
// //     }
// //     type = _type;
// //     flags = _flags;
// //     ndim = _ndim;
// //     char* buffer = buff_allocate(elem_size * hdc_sizeof(_type) + HDC_DATA_POS);
// //     buff_set_header(buffer, type, flags, ndim, shape);
// //     uuid = generate_uuid_str();
// //     storage = global_storage;
// //     storage->set(uuid,buffer,elem_size * hdc_sizeof(_type) + HDC_DATA_POS);
}

/** Copy contructor */
HDC::HDC(HDC* h) {
    printf("fixme\n");
    exit(7);
    storage = h->storage;
    memcpy(&header,h->get_buffer(),sizeof(header_t));
    uuid = generate_uuid_str();
    map_t* h_children = h->get_children_ptr();
    /*
    auto h_type = h->get_type();
    if (h_children != nullptr) {
        set_type(h_type);
        switch (h_type) {
            case (HDC_STRUCT):
            {
                for (map_t::iterator it : h_children) {
                    string key = it->key;
                    HDC* node = storage->get();
                    add_child(key,new HDC(node));
                    #ifdef DEBUG
                    cout << key << " copied" << endl;
                    #endif
                }
                break;
            }
            case (HDC_LIST):
            {
                for (size_t i = 0; i < h_children->size(); i++) {
                    HDC* node = (h_children->get<by_index>()[i].address.c_str());
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
    }*/
    memcpy(&header,h->get_buffer(),sizeof(header_t));
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

/** Deserializing constructor */
HDC::HDC(HDCStorage* _storage, string _uuid) {
    uuid = _uuid;
    storage = _storage;
    char* buffer = storage->get(uuid);
    memcpy(&header,buffer,sizeof(header_t));
}

/** Destructor */
HDC::~HDC() {
    storage->remove(uuid);
}

//---------------------------- Header information ----------------------------------
size_t HDC::get_datasize() {
    return header.data_size;
}
size_t HDC::get_size() {
    return header.buffer_size;
}
/** Returns type of current node. */
size_t HDC::get_type() {
    return header.type;
}
size_t HDC::get_flags()
{
    return header.flags;
}
template<typename T>
T* HDC::get_data() {
    return reinterpret_cast<T*>(get_buffer()+sizeof(header_t)*1);
}

bool HDC::is_external() {
    return (header.flags & HDCExternal) != 0;
}
bool HDC::is_readonly() {
    return (header.flags & HDCReadOnly) != 0;
}
bool HDC::is_fortranorder() {
    return (header.flags & HDCFortranOrder) != 0;
}
void HDC::info() {
    printf("Size:\t\t%zu\n",header.buffer_size);
    printf("NDim:\t\t%d\n",get_ndim());
    printf("Shape:\t\t"); for (int i=0;i<HDC_MAX_DIMS;i++) printf("%zu,",get_shape()[i]);
    printf("\n");
    printf("Data Size:\t\t%zu\n",get_datasize());
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
    for (size_t i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(")\n");
    #endif
    if(vs.empty()) return false; //TODO: re-do this!!!
    string first = vs[0];
    vs.erase(vs.begin());
    map_t* children = get_children_ptr();
    if (children == nullptr) return false;
    if (children->count(first.c_str()) == 0) return false;

    if (vs.empty()) {
        return (bool)children->count(first.c_str());
    } else {
        try {
            auto it = children->find(first.c_str());
            if (it != nullptr) {
                return get(first)->has_child(vs);
            }
            else return false; // TODO Create error HDC obj here.
        } catch (std::exception& e) {
            std::cerr << "has_child(): Caught exception: " << e.what() << "\n";
            return false;
        }
    }
    return false; // never goes here
}

void HDC::add_child(vector<string> vs, HDC* n) {
    #ifdef DEBUG
    printf("add_child(");
    for (long i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(")\n");
    #endif
    std::cout << "add_child() type: " << header.type << "\n";
    if (!(header.type == EMPTY_ID || header.type == STRUCT_ID)) {
        cout << "Cannot add child to this node. Data assigned???" << endl;
        return;
    }
    if (header.type == EMPTY_ID) set_type(STRUCT_ID);
    
    string first = vs[0];
    vs.erase(vs.begin());
    char* buffer = storage->get(uuid);
    auto segment = bip::managed_external_buffer(bip::open_only,buffer+sizeof(header_t)*1,0);
    printf("segment opened, size: %lu\n",segment.get_size());
    map_t* children = segment.find<map_t>("d").first;
    
    auto ca = segment.get_segment_manager();
    
    if (vs.empty()) {
        if (children->count(first.c_str()) == 0) {
            children->insert(record(first.c_str(),n->get_uuid().c_str(),ca));
        }
        else cout << "Error: child already exists!" << endl;
    } else {
        // Create new intermediate node
        if (children->count(first.c_str()) == 0) {
            printf("\n\n--- Creating new\n\n");
            HDC* nn = new HDC();
            children->insert(record(first.c_str(),nn->get_uuid().c_str(),ca));
            nn->add_child(vs,n);
        } else get(first)->add_child(vs,n);
    }
    storage->set(uuid,buffer,header.buffer_size);
    return;
}

vector<string> HDC::keys() {
    vector<string> k;
    map_t* children = get_children_ptr();
    if (children == nullptr) {
        return k;
    }
    k.reserve(children->size());

    for (map_t::iterator it = children->begin(); it != children->end(); ++it) {
        k.push_back(it->key.c_str());
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
    for (size_t i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(")\n");
    #endif
    if (!has_child(vs) || vs.empty())  {
        return;
    }
    string first = vs[0];
    vs.erase(vs.begin());
    map_t* children = get_children_ptr();
    if (vs.empty()) {
        auto it = children->find(first.c_str());
        if(it==children->end())
        children->erase(it);
    } else {
        get(first.c_str())->delete_child(vs);
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
    for (size_t i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(")\n");
    #endif
    string first = vs[0];
    vs.erase(vs.begin());
    char* buffer = storage->get(uuid);
    header_t h;
    memcpy(&h,buffer,sizeof(header_t));
    
    auto segment = bip::managed_external_buffer(bip::open_only,buffer+sizeof(header_t)*1,0);

    map_t* children = segment.find<map_t>("d").first;

    if (children->count(first.c_str())) {
        printf("a\n");
        string child_uuid = children->find(first.c_str())->address.c_str();
        if (vs.empty()) {
            return new HDC(storage,child_uuid);
        }
        else {
            HDC* child = new HDC(storage,child_uuid);
            return child->get(vs);
        }
    } else {
        cout << "Not found" << endl;
        exit(50);
        return new HDC();
    }
}

HDC* HDC::get_slice(vector<string> vs, size_t i) {
    #ifdef DEBUG
    printf("get_slice(");
    for (size_t i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(",%d)\n",i);
    #endif
    string first = vs[0];
    vs.erase(vs.begin());
    map_t* children = get_children_ptr();
    if (children->count(first.c_str())) {
        if (vs.empty()) {
            if (header.type != HDC_LIST) return this;
            if (i > children->size()) {
                cout << "Error: index out of range!" << endl;
                exit(50);
                return new HDC();
            }
            return new HDC(storage,children->get<by_index>()[i].address.c_str());
        }
        else return get(first)->get_slice(vs,i);
    } else {
        printf("Not found: get_slice(");
        for (size_t i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
        printf("%s",vs[vs.size()-1].c_str());
        printf(",%zu)\n",i);
        return new HDC();
    }
    return new HDC();
}


HDC* HDC::get_slice(size_t i) {
    #ifdef DEBUG
    printf("get_slice(%d)\n",i);
    #endif
    map_t* children = get_children_ptr();
    if (header.type == LIST_ID) return new HDC(storage,children->get<by_index>()[i].address.c_str());
    return this; // return this if not list
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
    for (size_t i = 0; i < vs.size()-1; i++) printf("%s/",vs[i].c_str());
    printf("%s",vs[vs.size()-1].c_str());
    printf(")\n");
    #endif
    if (!has_child(vs)) { // Nothing to set
        cout << "Nothing to set, maybe you want to add..." << endl;
        return;
    }
    string first = vs[0];
    vs.erase(vs.begin());
    bip::managed_external_buffer::allocator<record>::type ca = get_segment().get_allocator<record>();
    map_t* children = get_children_ptr();
    if (vs.empty()) {
        // check whether children container exists
        if (children == nullptr) children = get_children_ptr();
        // check whether given path exists
        if (children->count(first.c_str())) {
            shared_string str(get_uuid().c_str(),ca); // TODO wrap this up to try catch block
            children->modify(children->find(first.c_str()), change_node(str));
        } else {
            // TODO: get_allocator -- viz vyse...
            children->insert(record(first.c_str(),n->get_uuid().c_str(),ca));
        }
    } else get(first)->set_child(vs, n);
    return;
}


void HDC::set_child(string path, HDC* n) {
    set_child(split(path,'/'), n);
    return;
}

//------------------ Data manipulation -------------------------------


void HDC::set_type(size_t _type) {
    // More to be added here later
    #ifdef DEBUG
    printf("set_type(%d -> %d)\n",header.type,_type);
    #endif
    header.type = _type;
//     printf("uuid: %s\n",uuid);
//     printf("has %s: %d\n",uuid,storage->has(uuid));
    char* buffer = storage->get(uuid);
    if ((_type == LIST_ID || _type == STRUCT_ID)) {
        // TODO: increase buffer size first
//         printf("Assumed size: %d\n",header.buffer_size-sizeof(header_t)*1);
        auto segment = bip::managed_external_buffer(bip::create_only,buffer+sizeof(header_t)*1,header.buffer_size-sizeof(header_t)*1);
//         cout << "size: " << segment.get_size() << "\n";
//         cout << "header size: " << header.buffer_size << "\n";
        map_t* children = segment.construct<map_t>("d")(map_t::ctor_args_list(),map_t::allocator_type(segment.get_segment_manager()));
    } else printf("Skipping\n");
    memcpy(buffer,&header,sizeof(header_t));
    storage->set(uuid,buffer,header.buffer_size);
    return;
}

void* HDC::as_void_ptr() {
    return (void*)this;
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
// //     #ifdef DEBUG
// //     cout << "Called copy()" << endl;
// //     #endif
// //     HDC* _copy = new HDC();
// //     _copy->set_type(get_type());
// //     if (hdc_is_primitive_type(type)) {
// //         if (copy_arrays == 1) {
// //             _copy->set_buffer(buff_copy(storage->get(uuid)));
// //         } else return _copy;
// //     }
// //     else {
// //         switch (type) {
// //             case (EMPTY_ID):
// //                 // Nothing to be done
// //                 break;
// //             case (STRUCT_ID):
// //             {
// //                 for (size_t i=0;i<children->size();i++) {
// //                     string key = children->get<by_index>()[i].key.c_str();
// //                     #ifdef DEBUG
// //                     cout << "copy: " << key << endl;
// //                     #endif
// //                     HDC* node = get(storage->get(it->address.c_str()));
// //                     _copy->add_child(key,node->copy(copy_arrays));
// //                     #ifdef DEBUG
// //                     cout << key << " copied" << endl;
// //                     #endif
// //                 }
// //                 break;
// //             }
// //             case (LIST_ID):
// //             {
// //                 for (size_t i = 0; i < children->size(); i++)
// //                     _copy->insert_slice(i,get(storage->get(it->address.c_str()))->copy(copy_arrays));
// //                 break;
// //             }
// //             case (ERROR_ID):
// //             {
// //                 _copy->set_buffer(get_buffer());
// //                 break;
// //             }
// //             default:
// //             {
// //                 fprintf(stderr,"copy(): Unsupported type of node: %s\n",get_type_str().c_str());
// //                 exit(-3);
// //             }
// //         }
// //     }
// //     #ifdef DEBUG
// //     cout << "copy() Done" << endl;
// //     #endif
// //     return _copy;
}
void HDC::set_data_c(int _ndim, size_t* _shape, void* _data, size_t _type) {
// //     #ifdef DEBUG
// //     printf("set_data_c(%d, {%d,%d,%d}, %f, %s)\n",_ndim,_shape[0],_shape[1],_shape[2],((double*)_data)[0],hdc_get_type_str(_type).c_str());
// //     #endif
// //     if (storage->has(uuid)) storage->remove(uuid);
// //     type = _type;
// //     size = hdc_sizeof(type);
// //     ndim = _ndim;
// //     memset(shape,0,HDC_MAX_DIMS*sizeof(size_t));
// //     for (int i = 0; i < _ndim; i++) {
// //         size *= _shape[i];
// //         shape[i] = _shape[i];
// //     }
// //     char* buffer = buff_allocate(size+HDC_DATA_POS);
// //     buff_set_header(buffer,type,flags,ndim,shape);
// //     memcpy(buffer+HDC_DATA_POS,_data,size);
// //     storage->set(uuid,buffer,size+HDC_DATA_POS);
// //     return;
}
void HDC::set_data_c(string path, int _ndim, size_t* _shape, void* _data, size_t _type) {
    if(!has_child(path)) add_child(path, new HDC()); // TODO: add contructor for this!!
    get(path)->set_data_c(_ndim, _shape, _data, _type);
}

void HDC::insert_slice(size_t i, HDC* h)
{
    #ifdef DEBUG
    printf("insert_slice(%d)\n",i);
    #endif
    auto children = get_children_ptr();
    map_t::nth_index<1>::type& ri=children->get<1>();
    switch (header.type) {
        case EMPTY_ID:
            set_type(LIST_ID);
        case LIST_ID:
//            ri.insert(ri.begin()+i,{"_",h});
            ri.insert(ri.begin()+i,record("_",h->get_uuid().c_str(),get_segment().get_allocator<char>()));
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
    auto children = get_children_ptr();
    shared_string str(get_uuid().c_str(),get_segment().get_allocator<char>()); // TODO try-catch this (or make factory object/function???)
    switch (header.type) {
        case LIST_ID:
            if (i >= children->size()) {
                cout << "Error in set_slice: Index " << i << " >= list size." << endl;
                return;
            }
            children->modify(children->iterator_to(children->get<by_index>()[i]), change_node(str));
            break;
            //TODO: Add support for HDC_STRUCT here? How?
        default:
            cout << "Error in insert_slice(): Wrong type to call insert_slice." << endl;
            break;
    }
    return;
}

void HDC::append_slice(HDC* h) {
        insert_slice(header.shape[0]++,h);
    return;
}

hdc_t* HDC::as_hdc_ptr() {
    hdc_t* wrap = new struct hdc_t;
    wrap->obj = (void*) this;
    return wrap;
}

string HDC::get_type_str() {
    //return hdc_get_type_str(header.type);
    return "fixme";
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
    return header.ndim;
}

size_t* HDC::get_shape() {
    return header.shape;
}

int HDC::get_ndim(string path) {
    //TODO: make more error-proof - add has check -> make it as function???
    return get(path)->get_ndim();
}

size_t* HDC::get_shape(string path) {
    return get(path)->get_shape();
}

void HDC::set_buffer(char* _buffer) {
    printf("fixme");
    exit(7);
// //     if (storage->has(uuid)) {
// //         storage->remove(uuid);
// //     }
// //     //uuid = generate_uuid_str();
// //     type = buff_get_type(_buffer);
// //     flags= buff_get_flags(_buffer);
// //     size = buff_get_data_size(_buffer);
// //     ndim = buff_get_ndim(_buffer);
// //     memset(shape,0,HDC_MAX_DIMS*sizeof(size_t));
// //     memcpy(shape,buff_get_shape(_buffer),sizeof(size_t)*ndim);
// //     storage->set(uuid,_buffer,size);
// //     return;
}

size_t HDC::childs_count()
{
    return get_children_ptr()->size();
}

char* HDC::get_buffer() {
    return storage->get(uuid);
}

string HDC::get_uuid() {
    return uuid;
}

// allocator stuff
bip::managed_external_buffer HDC::get_segment() {
    char* buffer = storage->get(uuid);
    bip::managed_external_buffer segment;
    // attach managed buffer
    try {
        cout << "opening" << endl;
        segment = bip::managed_external_buffer(bip::open_only, buffer+sizeof(header_t)*1, 0);
        cout << "opened" << endl;
    }
    catch (std::exception& e) {
        cout << "creating" << endl;
        std::cerr << "get_segment(): Caught: " << e.what() << "\n";
        exit(1);
        segment = bip::managed_external_buffer(bip::create_only, buffer+sizeof(header_t)*1, header.buffer_size-sizeof(header_t)*1);
    }
    // FIXME: not necessary
    return segment;
}

map_t* HDC::get_children_ptr() {
    if (header.type != HDC_STRUCT && header.type != HDC_LIST) {
        std::cerr << ("get_children_ptr(): the type is not list or struct\n");
        exit(1);
    }
    char* buffer = storage->get(uuid);
    auto segment = bip::managed_external_buffer(bip::open_only, buffer+sizeof(header_t)*1, header.buffer_size-sizeof(header_t)*1);
    printf("size of segment: %d\n",segment.get_size());
    return segment.find<map_t>("d").first;
}


/*
bool HDC::get_flag(uint pos) {
    return (buff_get_flags(get_buffer()) >> pos & 1);
    return 
}

void HDC::set_flag(uint pos, bool val) {
    char* buffer = buff_copy(get_buffer());
    Flags flags = buff_get_flags(buffer);
    if (val)
        flags |= 1 << pos;
    else
        flags &= ~(1 << pos);
    buff_set_flags(buffer,flags);
    cout << flags << " " << get_flag(pos) << endl;
    storage->set(uuid,buffer,size+HDC_DATA_POS);
    return;
}
*/
