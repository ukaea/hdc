#include "hdc.hpp"
#include <fstream>
#include <memory>
#include <glob.h>
#include <dlfcn.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string_regex.hpp>
//#define DEBUG
struct hdc_t {
    void* obj;
};

using namespace std;

HDCStorage* global_storage = nullptr;

unordered_map<string,string> avail_stores;

pt::ptree* options;

void HDC::parse_cmdline(int argc, const char *argv[]) {
    namespace po = boost::program_options;
    po::options_description desc("Allowed options:");
    desc.add_options()
        ("help", "produce help message")
        ("list-plugins", "list available storage plugins")
        ("storage", po::value<std::string>(),"use given storage plugin")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        exit(0);
    }

    if (vm.count("list-plugins")) {
        HDC::search_plugins();
        HDC::list_plugins();
        exit(0);
    }

    if (vm.count("storage")) {
        string plugin_name = vm["storage"].as<std::string>();
        options->put("storage_cmdline",plugin_name);
    }
}

void HDC::load_config(std::string configPath) {
    std::string delimiters(":");
    std::vector<std::string> parts;
    boost::trim_if(configPath, boost::is_any_of(delimiters));
    boost::split(parts, configPath, boost::is_any_of(delimiters),boost::token_compress_on);
    pt::ptree settings_read;
    bool config_found = false;
    for (auto path : parts) {
        if (boost::filesystem::exists(path)) {
            try {
                pt::read_json(path,settings_read);
                config_found = true;
                break;
            }
            catch (...) {
                cout << "HDC::load_config(): something bad happened" << endl;
            }
        }
    }
    if (config_found) {
        options->put("storage",settings_read.get<std::string>("storage","umap"));
        boost::optional<pt::ptree&> s = settings_read.get_child_optional("storage_options");
        if (s.is_initialized())
            options->add_child("storage_options",s.value());
//         else
//             options->add_child("storage_options",pt::ptree());
    }
}

void HDC::search_plugins(string searchPath) {
    std::string delimiters(":");
    std::vector<std::string> parts;
    boost::trim_if(searchPath, boost::is_any_of(delimiters));
    boost::split(parts, searchPath, boost::is_any_of(delimiters),boost::token_compress_on);
    std::string lib_dir = HDC::get_library_dir();
    parts.push_back(lib_dir);
    parts.push_back(lib_dir+"/plugins");
    parts.push_back(lib_dir+"/hdc");
    glob_t globbuf;
    for (auto path : parts) {
        //cout << path << " " << endl;
        string tmp_pth = path+"/libhdc_plugin_*.so*";
        int err = glob(tmp_pth.c_str(),0,NULL,&globbuf);
        if (err == 0) {
        for (size_t i = 0; i < globbuf.gl_pathc; i++)
        {
            string pluginPath = globbuf.gl_pathv[i];
            //cout << pluginPath << endl;
            // try to load the plugin and if it runs if succeeds, bring it into map_t
            try {
                HDCStorage stor(globbuf.gl_pathv[i],"{\"do_not_init\": true}");
                string name = stor.name();
                if (!name.empty()) {
                    if (avail_stores.find(name)==avail_stores.end())
                        avail_stores[name] = pluginPath;
                }
            } catch(...) {
                cout << "Nope\n";
            }
        }
        globfree(&globbuf);
        }
    }
};

std::vector<std::string> HDC::get_available_plugins() {
    std::vector<std::string> keys;
    for (auto kv : avail_stores) {
        keys.push_back(kv.first);
    }
    return keys;
}

void HDC::list_plugins() {
    cout << "Available storage plugins:\n";
    for (const auto& store : avail_stores) {
        cout << "  - " << store.first << " : " << store.second << "\n";
    }
}

void HDC::set_storage(std::string storage) {
    boost::optional<std::string> storage_cmd =  options->get_optional<std::string>("storage_cmdline");
    if (storage_cmd) {
        while (options->count("storage") > 0) options->erase("storage");
        options->put("storage",storage_cmd.value());
        while (options->count("storage_cmdline") > 0) options->erase("storage_cmdline");
    }
    string selected_store_name = options->get<std::string>("storage",storage);
    //cout << "Selected storage: " << selected_store_name <<endl;
    if (avail_stores.find(selected_store_name) != avail_stores.end()) {
        //cout << avail_stores[selected_store_name] << endl;
        if (!options->count("storage_options")) options->add_child("storage_options",pt::ptree());
        global_storage = new HDCStorage(avail_stores[selected_store_name],options->get_child("storage_options"));
    } else {
        throw HDCException("Unable to select the store.\n");
    }
}

void HDC::set_default_storage_options(std::string storage, std::string storage_options) {
    options->put("storage",storage);
    if (!storage_options.empty()) {
        std::stringstream ss(storage_options);
        pt::ptree* parsed_stor_opt = new pt::ptree();
        pt::read_json(ss,*parsed_stor_opt);
        options->add_child("storage_options",*parsed_stor_opt);
    }
}

std::string HDC::get_library_dir(void) {
    Dl_info dl_info;
    dladdr((void *)HDC::get_library_dir, &dl_info);
    boost::filesystem::path p(dl_info.dli_fname);
    return boost::filesystem::canonical(p.parent_path()).string();
}

void HDC::init(std::string storage_str, std::string storage_options) {
    options = new pt::ptree();
    HDC::set_default_storage_options(storage_str,storage_options);
    HDC::search_plugins();
    //HDC::load_config();
    HDC::set_storage(storage_str);
}


/** Cleans up global_storage  -- mainly due to C and Fortran */
void HDC::destroy() {
    if (global_storage == nullptr) return;
    delete global_storage;
    global_storage = nullptr;
    delete options;
    options = nullptr;
    DEBUG_STDERR("HDC::destroy(): HDC storage destroyed.\n");
}

//---------------------------- HDC class -----------------------------------

/** Creates empty HDC with specified buffer size */
HDC::HDC(size_t _data_size) {
    // fill some data
    memset(&header,0,sizeof(header_t));
    header.buffer_size = _data_size + sizeof(header_t);
    header.data_size = _data_size;
    header.ndim = 1;

    if (global_storage == nullptr) {
       //HDC::init("./plugins/libMDBMPlugin.so","./plugins/settings.txt");
       HDC::init();
       atexit(HDC::destroy);
    }

    // Start by creating segment
    char* buffer = new char[header.buffer_size];

    // copy header there -- we need that, hopefully it will be optimized out
    memcpy(buffer,&header,sizeof(header_t));

    //Store to some storage
    uuid = generate_uuid_str();
    storage = global_storage;
    storage->set(uuid,buffer,header.buffer_size);
    // Now it is safe to
    if (!storage->usesBuffersDirectly()) delete[] buffer;
}

/** Default constructor. Creates empty HDC */
HDC::HDC(): HDC(0lu) {};

/** Creates empty HDC with specified type and shape */
HDC::HDC(int _ndim, size_t* _shape, TypeID _type,long _flags) {
    if (_ndim >= HDC_MAX_DIMS) {
        throw HDCException("HDC(): Unsupported number of dimensions: "+to_string(_ndim));
    }
    size_t elem_size = 1;
    memset(&header,0,sizeof(header_t));
    for (int i = 0; i < _ndim; i++) {
        header.shape[i] = _shape[i];
        elem_size *= _shape[i];
    }
    header.type = _type;
    header.flags = _flags;
    header.ndim = _ndim;
    header.data_size = elem_size * hdc_sizeof(_type);
    header.buffer_size = header.data_size + sizeof(header_t);
    char* buffer = new char[header.buffer_size];
    memcpy(buffer,&header,sizeof(header_t));
    uuid = generate_uuid_str();
    storage = global_storage;
    storage->set(uuid,buffer,header.buffer_size);
    if (!storage->usesBuffersDirectly()) delete[] buffer;
}

/** Creates a new HDC instance from a given string. If a supplied string contains uri, it tries to open a given resource */
HDC::HDC(const std::string str): HDC() {
    // start by parsing the string
    std::vector<std::string> result;
    boost::algorithm::split_regex( result, str, boost::regex( "://" ) ) ;
    int i = 0;
    if (result.size() > 1) {
        std::vector<std::string> split_res;
        boost::split( split_res, result[1], boost::is_any_of("|"), boost::token_compress_on );
        if (split_res.size() == 1) split_res.push_back("");
        auto prefix = result[0];
        if (prefix == "hdf5") {
            HDC h = from_hdf5(split_res[0],split_res[1]);
            memcpy(&(this->header),h.get_buffer(),sizeof(header_t));
            uuid = h.get_uuid();
            storage = global_storage;
        } else if (prefix == "json") {
            HDC h = from_json(split_res[0],split_res[1]);
            memcpy(&(this->header),h.get_buffer(),sizeof(header_t));
            uuid = h.get_uuid();
            storage = global_storage;
        } else if (prefix == "uda") {
            HDC h = from_uda(split_res[0],split_res[1]);
            memcpy(&(this->header),h.get_buffer(),sizeof(header_t));
            uuid = h.get_uuid();
            storage = global_storage;
        } else {
            throw HDCException("Protocol "+prefix+" not known\n");
        }
    } else {
        // fill some data
        memset(&header,0,sizeof(header_t));
        size_t _data_size = str.length()+1;
        header.buffer_size = _data_size + sizeof(header_t);
        header.data_size = _data_size;
        header.ndim = 1;
        header.type = STRING_ID;

        if (global_storage == nullptr) {
            HDC::init();
            atexit(HDC::destroy);
        }

        // Start by creating segment
        char* buffer = new char[header.buffer_size];

        // copy header there -- we need that, hopefully it will be optimized out
        memcpy(buffer,&header,sizeof(header_t));
        // Copy char* data
        memcpy(buffer+sizeof(header_t),str.c_str(),header.data_size);
        //Store to some storage
        uuid = generate_uuid_str();
        storage = global_storage;
        storage->set(uuid,buffer,header.buffer_size);
        // Now it is safe to
        if (!storage->usesBuffersDirectly()) delete[] buffer;
    }
}

HDC::HDC(void* src_buffer) {
    storage = global_storage;
    uuid = generate_uuid_str();
    memcpy(&header,src_buffer,sizeof(header_t));
    auto buffer = new char[header.buffer_size];

    if (header.type == STRUCT_ID || header.type == LIST_ID) {
        bip::managed_external_buffer src_segment(bip::open_only,src_buffer+sizeof(header_t),header.buffer_size-sizeof(header_t));
        map_t* src_children = src_segment.find<map_t>("d").first;
        if (src_children == nullptr) {
            throw HDCException("HDC(): The node has no children.\n");
        }
        bip::managed_external_buffer segment(bip::create_only,buffer+sizeof(header_t),header.buffer_size-sizeof(header_t));
        auto children = segment.construct<map_t>("d")(map_t::ctor_args_list(),map_t::allocator_type(segment.get_segment_manager()));
        for (map_t::iterator it = src_children->get<by_key>().begin(); it != src_children->get<by_key>().end(); ++it) {
            HDC n(storage,it->address.c_str());
            record rec(it->key.c_str(),n.get_uuid().c_str(),segment.get_segment_manager());
            children->insert(rec);
        }
        memcpy(buffer,&header,sizeof(header_t));
    } else {
        memcpy(buffer,src_buffer,header.buffer_size);
    }

    storage->set(uuid,buffer,header.buffer_size);
}

/** Copy contructor */
HDC::HDC(HDC* h) : HDC(h->get_buffer()) {};

/** Deserializing constructor */
HDC::HDC(HDCStorage* _storage, const std::string& _uuid) {
    uuid = _uuid;
    storage = _storage;
    memcpy(&header,storage->get(uuid),sizeof(header_t));
}

/** Destructor */
HDC::~HDC() {
    //cout << "destructor called\n";
    /*map_t* children;
    try {
        children = get_children_ptr();
    } catch(...) {
        return;
    };
    for (map_t::iterator it = children->begin(); it != children->end(); ++it) {
        storage->remove(it->address.c_str());
    }*/
    //storage->remove(uuid); // This is responsibility of storage from now
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
    return reinterpret_cast<T*>(get_buffer()+sizeof(header_t));
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

bool HDC::has_child(const std::string& path)
{
    return has_child(split(path));
}

bool HDC::has_child(vector<boost::variant<size_t,std::string>> vs)
{
    D(
    printf("has_child(");
    for (size_t i = 0; i < vs.size()-1; i++) {
        cout << vs[i] << "/";
    }
    cout << vs[vs.size()-1];
    cout << ")\n";
    )
    if(vs.empty()) {
        return false; //TODO: re-do this!!!
    }
    if (header.type != HDC_STRUCT && header.type != HDC_LIST) {
        return false;
    }
    auto first = vs[0];
    vs.erase(vs.begin());

    map_t* children = get_children_ptr();
    if (children == nullptr) return false;
    if (first.type() == typeid(std::string)) {
        if (children->count(boost::get<std::string>(first).c_str()) == 0) return false;
    }
    // if parse succeeds call get_slice instead
    //size_t index;
    //bool is_index = try_parse(first,index);
    if (vs.empty()) {
        if (first.type() == typeid(size_t)) {
            return (children->size() > boost::get<size_t>(first));
        } else
            return (bool)children->count(boost::get<std::string>(first).c_str());
    } else {
        if (first.type() == typeid(size_t)) {
            try {
                auto it = children->get<by_index>()[boost::get<size_t>(first)];
                HDC ch(storage,it.address.c_str());
                return ch.has_child(vs);
            } catch (...) {
                std::cerr << "has_child(): Caught exception: index" << "\n";
                return false;
            }
        } else {
            try {
                auto it = children->find(boost::get<std::string>(first).c_str());
                if (it != children->end()) {
                    HDC ch(storage,it->address.c_str());
                    return ch.has_child(vs);
                }
                else return false; // TODO Create error HDC obj here???
            } catch (...) {
                std::cerr << "has_child(): Caught exception: string" << "\n";
                return false;
            }
        }
    }
    return false; // never goes here
}

void HDC::add_child(vector<boost::variant<size_t,std::string>> vs, HDC* n) {
    add_child(vs,*n);
    return;
}

void HDC::add_child(vector<boost::variant<size_t,std::string>> vs, HDC& n) {
    auto first = vs[0];

    vs.erase(vs.begin());

    if (!vs.empty()) { // Create intermediate nodes here
        HDC h;
//         cout << " -- " << first << endl;
        if (first.type() == typeid(size_t)) {
            auto index = boost::get<size_t>(first);
            if (get_shape()[0] == index) insert_slice(index,h); // TODO: can be simplified by introducing get(boost::variant)
            get_slice(boost::get<size_t>(first)).add_child(vs,n); // TODO: get_slice is not working for now...
        } else {
            add_child(boost::get<std::string>(first),h);
            get(boost::get<std::string>(first)).add_child(vs,n);
        }
    } else {
        if (first.type() == typeid(size_t)) {
            size_t index = boost::get<size_t>(first);
            if (get_shape()[0] == index) insert_slice(index,n);
            else if (get_shape()[0] < index) {
                throw HDCException("add_child(): Supplied index greater than maximal admissible)\n");
            }
        } else {
            if (boost::get<std::string>(first).size() > 1024) {
                throw HDCException("add_child(): string too long.\n");
            }
            add_child_single(boost::get<std::string>(first),n);
        }
    }
    return;
}

void HDC::add_child_single(const std::string& path, HDC& n) {
    D(
    cout << "add_child_single("+path+")\n";
    )
    // sync buffer
    auto buffer = storage->get(uuid);
    memcpy(&header,buffer,sizeof(header_t));
    int old_size = header.buffer_size;
    if (!(header.type == EMPTY_ID || header.type == STRUCT_ID)) {
        throw HDCException("add_child_single(): Cannot add child to this node. Data assigned???\n");
    }
    if (header.type == EMPTY_ID) set_type(STRUCT_ID);

    if (path.size() > 1024) {
        throw HDCException("add_child_single(): string too long.\n");
    }

    // load new buffer
    buffer = storage->get(uuid);
    memcpy(&header,buffer,sizeof(header_t));

    bip::managed_external_buffer segment(bip::open_only,buffer+sizeof(header_t),0);
    auto children = segment.find<map_t>("d").first;

    if (children->count(path.c_str()) == 0) {
        // Try to grow buffer HDC_MAX_RESIZE_ATTEMPTS times, die if it does not help
        int redo = 1;
        for (int i=0;i<HDC_MAX_RESIZE_ATTEMPTS-1;i++) {
            if (redo == 0) break;
            try {
                // Storing record larger than 114 bytes can mess up the heap on small buffers (1300b, key with longer than 24 characters), so we need to increase the buffer size here
                if (segment.get_free_memory() < 4*path.size()) {
                    throw (HDCBadAllocException()); // There can be problem to store large strings
                }
                record rec(path.c_str(),n.get_uuid().c_str(),segment.get_segment_manager());
                children->insert(rec);
                redo = 0;
            }
            catch (HDCBadAllocException e) {
                char* new_buffer = buffer_grow(buffer,max(header.buffer_size,4*path.size()));
                if (new_buffer == buffer) {
                    throw HDCException("grow called, but buffer == new_buffer.\n");
                }
                //if (!storage->usesBuffersDirectly()) delete[] buffer;
                storage->remove(uuid);
                buffer = new_buffer;
                memcpy(&header,buffer,sizeof(header_t));
                segment = bip::managed_external_buffer(bip::open_only,buffer+sizeof(header_t),0);
                children = segment.find<map_t>("d").first;
                redo = 1;
            }
            if (redo == 1 && i == HDC_MAX_RESIZE_ATTEMPTS-1) {
                throw HDCBadAllocException("add_child_single(): Could not allocate enough memory.\n");
            }
        }

        if (header.buffer_size != old_size) {
            storage->set(uuid,buffer,header.buffer_size);
        }
    }
    return;
}

vector<string> HDC::keys() {
    vector<string> k;
    map_t* children;
    children = get_children_ptr();
    if (children == nullptr) return k;
    k.reserve(children->size());

    for (map_t::iterator it = children->begin(); it != children->end(); ++it) {
        k.push_back(it->key.c_str());
    }
    return k;
}

void HDC::add_child(const std::string& path, HDC* n)
{
    DEBUG_STDOUT("add_child("+path+")\n");
    add_child(split(path),n);
    return;
}

void HDC::add_child(const std::string& path, HDC& n)
{
    DEBUG_STDOUT("add_child("+path+")\n");
    add_child(split(path),n);
    return;
}

void HDC::delete_child(vector<boost::variant<size_t,std::string>> vs) {
    D(
    cout << "delete_child(";
    for (size_t i = 0; i < vs.size()-1; i++) {
        cout << vs[i] << "/";
    }
    cout << vs[vs.size()-1];
    cout << ")\n";
    )
    if (!has_child(vs) || vs.empty())  {
        return;
    }
    auto first = vs[0];
    vs.erase(vs.begin());
    map_t* children = get_children_ptr();
    if (vs.empty()) {
        //size_t index;
        if (first.type() == typeid(size_t)) {
            auto it = children->get<by_index>()[boost::get<size_t>(first)];
            storage->remove(it.address.c_str());
            // Maybe this should not be supported...
        } else {
            auto it = children->find(boost::get<std::string>(first).c_str());
            if (it!=children->end()) {
                storage->remove(it->address.c_str());
                children->erase(it);
            }
        }
    } else {
        get(boost::get<std::string>(first)).delete_child(vs);
    }
    // set type back to empty if the only child was deleted.
    //if (children->empty()) set_type(EMPTY_ID); Not sure if to do this
    return;
}

void HDC::delete_child(const std::string& path) {
    delete_child(split(path));
    return;
}

HDC* HDC::get_ptr(vector<boost::variant<size_t,std::string>> vs) {
    D(
    cout << "get_ptr(";
    for (size_t i = 0; i < vs.size()-1; i++) {
        cout << vs[i] << "/";
    }
    cout << vs[vs.size()-1];
    cout << ")\n";
    )
    auto first = vs[0];
    vs.erase(vs.begin());
    if (first.type() == typeid(size_t)) return get_slice_ptr(boost::get<size_t>(first));

    char* buffer = storage->get(uuid);
    header_t h;
    memcpy(&h,buffer,sizeof(header_t));
    bip::managed_external_buffer segment(bip::open_only,buffer+sizeof(header_t),0);
    map_t* children = segment.find<map_t>("d").first;
    if (children == nullptr) {
        throw HDCException("get_ptr(): This node has no children.\n");
    }
    if (children->count(boost::get<std::string>(first).c_str())) {
        auto rec = children->find(boost::get<std::string>(first).c_str());
        //cout << rec->key << " " << rec->address << "\n";
        string child_uuid = rec->address.c_str();
        if (vs.empty()) {
            return new HDC(storage,child_uuid);
        }
        else {
            HDC child(storage,child_uuid);
            return child.get_ptr(vs);
        }
    } else {
        throw HDCException("get_ptr(): Node not found\n");
    }
}

HDC HDC::get(vector<boost::variant<size_t,std::string>> vs) {
    D(
    cout << "get(";
    for (size_t i = 0; i < vs.size()-1; i++) {
        cout << vs[i] << "/";
    }
    cout << vs[vs.size()-1];
    cout << ")\n";
    )
    auto first = vs[0];
    vs.erase(vs.begin());
    char* buffer = storage->get(uuid);
    header_t h;
    memcpy(&h,buffer,sizeof(header_t));

    auto segment = bip::managed_external_buffer(bip::open_only,buffer+sizeof(header_t),0);

    map_t* children = segment.find<map_t>("d").first;
    if (children == nullptr) {
        throw HDCException("get(): This node has no children.");
    }
    if (first.type() == typeid(std::string)) {
        if (children->count(boost::lexical_cast<std::string>(first).c_str())) {
            auto rec = children->find(boost::lexical_cast<std::string>(first).c_str());
            string child_uuid = rec->address.c_str();
            if (vs.empty()) {
                HDC nn(storage,child_uuid);
                return nn;
            }
            else {
                HDC child(storage,child_uuid);
                return child.get(vs);
            }
        } else {
            throw HDCException("get(): Not found\n");
        }
    } else {
        return get_slice(boost::get<size_t>(first));
    }
}

HDC HDC::get_slice(vector<boost::variant<size_t,std::string>> vs, size_t i) {
    D(
    printf("get_slice(");
    for (size_t i = 0; i < vs.size()-1; i++) printf("%s/",boost::get<std::string>(vs[i]).c_str());
    printf("%s",boost::get<std::string>(vs[vs.size()-1]).c_str());
    printf(",%zu)\n",i);
    )
    auto first = vs[0];
    vs.erase(vs.begin());
    map_t* children = get_children_ptr();
    if (children->count(boost::get<std::string>(first).c_str())) {
        if (vs.empty()) {
            if (header.type != HDC_LIST) return HDC(storage,uuid);
            if (i > children->size()) {
                throw HDCException("get_slice(): index out of range!\n");
            }
            return HDC(storage,children->get<by_index>()[i].address.c_str());
        }
        else return get(boost::get<std::string>(first)).get_slice(vs,i);
    } else {
        throw HDCException("get_slice(): Not found\n");
    }
    return HDC();
}

HDC* HDC::get_slice_ptr(vector<boost::variant<size_t,std::string>> vs, size_t i) {
    D(
    cout << "get_slice(";
    for (size_t i = 0; i < vs.size()-1; i++) {
        cout << vs[i] << "/";
    }
    cout << vs[vs.size()-1];
    cout << ")\n";
    )
    auto first = vs[0];
    vs.erase(vs.begin());
    map_t* children = get_children_ptr();
    if (children->count(boost::get<std::string>(first).c_str())) {
        if (vs.empty()) {
            if (header.type != HDC_LIST) return this;
            if (i > children->size()) {
                throw HDCException("get_slice_ptr(): index out of range!\n");
            }
            return new HDC(storage,children->get<by_index>()[i].address.c_str());
        }
        else return get(boost::get<std::string>(first)).get_slice_ptr(vs,i);
    } else {
        throw HDCException("get_slice_ptr(): Not found\n");
    }
    return new HDC();
}

HDC HDC::get_slice(size_t i) {
    DEBUG_STDOUT("get_slice("+to_string(i)+")\n");
    map_t* children = get_children_ptr();
    if (header.type == LIST_ID) return HDC(storage,children->get<by_index>()[i].address.c_str());
    return this; // return this if not list
}

HDC* HDC::get_slice_ptr(size_t i) {
    DEBUG_STDOUT("get_slice("+to_string(i)+")\n");
    map_t* children = get_children_ptr();
    if (header.type == LIST_ID) {
        return new HDC(storage,children->get<by_index>()[i].address.c_str());
    }
    return this; // return this if not list
}

HDC HDC::get_slice(const std::string& path, size_t i) {
    return get_slice(split(path),i);
}

HDC* HDC::get_slice_ptr(const std::string& path, size_t i) {
    return get_slice_ptr(split(path),i);
}

HDC* HDC::get_ptr(const std::string& path) {
    return get_ptr(split(path));
}

HDC HDC::get(const std::string& path) {
    return get(split(path));
}

void HDC::set_child(vector<boost::variant<size_t,std::string>> vs, HDC* n) {
    D(
    cout << "set_child(";
    for (size_t i = 0; i < vs.size()-1; i++) {
        cout << vs[i] << "/";
    }
    cout << vs[vs.size()-1];
    cout << ")\n";
    )
    if (!has_child(vs)) { // Nothing to set
        cout << "Nothing to set, maybe you want to add..." << endl;
        return;
    }
    auto first = vs[0];
    vs.erase(vs.begin());
    bip::managed_external_buffer::allocator<record>::type ca = get_segment().get_allocator<record>();
    map_t* children = get_children_ptr();
    if (vs.empty()) {
        if (first.type() == typeid(size_t)) {
            set_slice(boost::get<size_t>(first), n);
        } else {
            // check whether children container exists
            if (children == nullptr) children = get_children_ptr();
            // check whether given path exists
            if (children->count(boost::get<std::string>(first).c_str())) {
                shared_string str(n->get_uuid().c_str(),ca); // TODO wrap this up to try catch block
                children->modify(children->find(boost::get<std::string>(first).c_str()), change_node(str));
            } else {
                // TODO: get_allocator -- viz vyse...
                children->insert(record(boost::get<std::string>(first).c_str(),n->get_uuid().c_str(),ca));
            }
        }
    } else get(boost::get<std::string>(first)).set_child(vs, n);
    return;
}


void HDC::set_child(const std::string& path, HDC* n) {
    set_child(split(path), n);
    return;
}

//------------------ Data manipulation -------------------------------


void HDC::set_type(size_t _type) {
    // More to be added here later
    DEBUG_STDOUT("set_type("+to_string(header.type)+" -> "+to_string(_type)+")\n");
    char* old_buffer = storage->get(uuid);
    memcpy(&header,old_buffer,sizeof(header_t)); //sync header
    if (header.type == _type) return; // Nothing to do
    header.type = _type;
    char* new_buffer;
    memcpy(old_buffer,&header,sizeof(header_t)); //sync header back
    if (header.type == STRUCT_ID || header.type == LIST_ID) {
        if (header.data_size < HDC_NODE_SIZE_DEFAULT) {
            new_buffer = new char[HDC_NODE_SIZE_DEFAULT];
            header.data_size = HDC_NODE_SIZE_DEFAULT - sizeof(header_t);
            header.buffer_size = HDC_NODE_SIZE_DEFAULT;
            memcpy(new_buffer,&header,sizeof(header_t));
        }
        else
            new_buffer = old_buffer;
        bip::managed_external_buffer segment(bip::create_only,new_buffer+sizeof(header_t),header.data_size);
        map_t* children = segment.construct<map_t>("d")(map_t::ctor_args_list(),map_t::allocator_type(segment.get_segment_manager())); // TODO: Wrap this to auto-growing???
    }
    // else there is nothing to do...
    storage->set(uuid,new_buffer,header.buffer_size);
    if (new_buffer != old_buffer)
    if (!storage->usesBuffersDirectly()) delete[] new_buffer;
    return;
}

intptr_t HDC::as_void_ptr() {
    return (intptr_t)(void*)this;
}

void HDC::dump() {
    cout << to_json(0) << endl;
}

/** Serializes HDC to special json file*/
void HDC::serialize(string filename) {
    ofstream file(filename);
    file << serialize();
    file.close();
}

string HDC::serialize() {
    pt::ptree root = this->storage->get_status();
    root.put("uuid",this->uuid);
    std::stringstream ss;
    write_json(ss, root);
    return ss.str();
}

void HDC::resize(HDC* h, int recursively)
{
    throw HDCException("resize(): not implemented yet...\n");
}

HDC* HDC::copy(int copy_arrays) {
    return new HDC(this);
}

void HDC::set_data_c(int _ndim, size_t* _shape, void* _data, size_t _type, Flags _flags) {
    D(printf("set_data_c(%d, {%d,%d,%d}, %f, %s)\n",_ndim,_shape[0],_shape[1],_shape[2],((double*)_data)[0],hdc_type_str(static_cast<TypeID>(_type)).c_str());)
    auto buffer = storage->get(uuid);
    memcpy(&header,buffer,sizeof(header_t));
    // Start with determining of the buffer size
    size_t data_size = hdc_sizeof(static_cast<TypeID>(_type));
    for (int i=0;i<_ndim;i++) data_size *= _shape[i];
    size_t buffer_size = data_size + sizeof(header_t);
    if (header.buffer_size == buffer_size) {
            storage->lock(uuid);
            memcpy(buffer+sizeof(header_t),_data,data_size);
            storage->unlock(uuid);
            return;
    } else {
        header.buffer_size = buffer_size;
        header.data_size = data_size;
        memset(header.shape,0,HDC_MAX_DIMS*sizeof(size_t));
        for (int i=0;i<_ndim;i++) header.shape[i] = _shape[i];
        header.type = static_cast<TypeID>(_type);
        header.ndim = _ndim;
        char* buffer = new char[header.buffer_size];
        memcpy(buffer,&header,sizeof(header_t));
        memcpy(buffer+sizeof(header_t),_data,header.data_size);
        storage->set(uuid,buffer,header.buffer_size);
        if (!storage->usesBuffersDirectly()) delete[] buffer;
        return;
    }
}

void HDC::set_data_c(const std::string& path, int _ndim, size_t* _shape, void* _data, size_t _type, Flags _flags) {
    if(!has_child(path)) {
        HDC h;
        add_child(path, h); // TODO: add constructor for this!!
    }
    get(path).set_data_c(_ndim, _shape, _data, _type);
}

void HDC::set_data_c(int _ndim, size_t* _shape, const void* _data, size_t _type, Flags _flags) {
    D(printf("set_data_c(%d, {%d,%d,%d}, %f, %s)\n",_ndim,_shape[0],_shape[1],_shape[2],((double*)_data)[0],hdc_type_str(static_cast<TypeID>(_type)).c_str());)
    auto buffer = storage->get(uuid);
    memcpy(&header,buffer,sizeof(header_t));
    // Start with determining of the buffer size
    size_t data_size = hdc_sizeof(static_cast<TypeID>(_type));
    for (int i=0;i<_ndim;i++) data_size *= _shape[i];
    size_t buffer_size = data_size + sizeof(header_t);
    if (header.buffer_size == buffer_size) {
            storage->lock(uuid);
            memcpy(buffer+sizeof(header_t),_data,data_size);
            storage->unlock(uuid);
            return;
    } else {
        header.buffer_size = buffer_size;
        header.data_size = data_size;
        memset(header.shape,0,HDC_MAX_DIMS*sizeof(size_t));
        for (int i=0;i<_ndim;i++) header.shape[i] = _shape[i];
        header.type = static_cast<TypeID>(_type);
        header.ndim = _ndim;
        char* buffer = new char[header.buffer_size];
        memcpy(buffer,&header,sizeof(header_t));
        memcpy(buffer+sizeof(header_t),_data,header.data_size);
        storage->set(uuid,buffer,header.buffer_size);
        if (!storage->usesBuffersDirectly()) delete[] buffer;
        return;
    }
}

void HDC::set_data_c(const std::string& path, int _ndim, size_t* _shape, const void* _data, size_t _type, Flags _flags) {
    if(!has_child(path)) {
        HDC h;
        add_child(path, h); // TODO: add constructor for this!!
    }
    get(path).set_data_c(_ndim, _shape, _data, _type);
}

void HDC::insert_slice(size_t i, HDC* h) {
    insert_slice(i,*h);
    return;
}

void HDC::insert_slice(size_t i, HDC& h)
{
    DEBUG_STDOUT("insert_slice("+to_string(i)+")\n");
    memcpy(&header,storage->get(uuid),sizeof(header_t));
    size_t old_size = header.buffer_size;
    if (header.type != HDC_EMPTY && header.type != HDC_LIST) {
        throw HDCException("insert_slice(): Wrong type to call insert_slice.\n");
    }
    if (header.type == HDC_EMPTY) set_type(LIST_ID);

    auto buffer = storage->get(uuid);
    memcpy(&header,buffer,sizeof(header_t));
    if (get_shape()[0] < i) {
        throw HDCException("insert_slice(): Supplied index larger than size\n");
    }

    bip::managed_external_buffer segment(bip::open_only,buffer+sizeof(header_t),0);
    auto children = segment.find<map_t>("d").first;

    int k = 0;
    int redo = 1;
    for (k=0;k<HDC_MAX_RESIZE_ATTEMPTS-1;k++) {
        if (redo == 0) break;
        try {
            map_t::nth_index<1>::type& ri=children->get<1>();
            ri.insert(ri.begin()+i,record(to_string(i).c_str(),h.get_uuid().c_str(),segment.get_segment_manager()));
            redo = 0;
        } catch (exception e) {
            char* new_buffer = buffer_grow(buffer, header.buffer_size);
            //if (!storage->usesBuffersDirectly()) delete[] buffer;
            storage->remove(uuid);
            buffer = new_buffer;
            memcpy(&header,buffer,sizeof(header_t));
            segment = bip::managed_external_buffer(bip::open_only,buffer+sizeof(header_t),0);
            children = segment.find<map_t>("d").first;
            redo = 1;
        }
    }
    if (redo == 1 && k >= HDC_MAX_RESIZE_ATTEMPTS-1) {
        throw HDCBadAllocException("insert_slice(): Could not allocate enough memory.\n");
    }
    header.shape[0] = children->size();
    memcpy(buffer,&header,sizeof(header_t));
    if (header.buffer_size != old_size) storage->set(uuid,buffer,header.buffer_size);
    return;
}

void HDC::set_slice(size_t i, HDC* h) {
    set_slice(i,*h);
    return;
}

void HDC::set_slice(size_t i, HDC& h)
{
    DEBUG_STDOUT("set_slice("+to_string(i)+")\n");
    if (header.type != LIST_ID) {
        throw HDCException("set_slice(): called on non list type node\n");
    };
    auto children = get_children_ptr();
    bip::managed_external_buffer::allocator<record>::type ca = get_segment().get_allocator<record>();
    shared_string str(h.get_uuid().c_str(),ca); // TODO try-catch this (or make factory object/function???)
    if (i >= children->size()) {
        throw HDCException("set_slice(): Index "+std::to_string(i)+" >= list size.\n");
    }
    children->modify(children->iterator_to(children->get<by_index>()[i]), change_node(str));
    return;
}

void HDC::append_slice(HDC* h) {
    append_slice(*h);
    return;
}

void HDC::append_slice(HDC& h) {
    memcpy(&header,storage->get(uuid),sizeof(header_t));
    insert_slice(header.shape[0],h);
    return;
}

hdc_t* HDC::as_hdc_ptr() {
    hdc_t* wrap = new struct hdc_t;
    wrap->obj = (void*) this;
    return wrap;
}

string HDC::get_type_str() {
    switch(header.type) {
        case EMPTY_ID:
            return "null";
        case STRUCT_ID:
            return "struct";
        case LIST_ID:
            return "list";
        case INT8_ID:
            return "int8";
        case INT16_ID:
            return "int16";
        case INT32_ID:
            return "int32";
        case INT64_ID:
            return "int64";
        case UINT8_ID:
            return "int8";
        case UINT16_ID:
            return "int16";
        case UINT32_ID:
            return "int32";
        case UINT64_ID:
            return "int64";
        case FLOAT_ID:
            return "float32";
        case DOUBLE_ID:
            return "float64";
        case STRING_ID:
            return "string";
        case BOOL_ID:
            return "bool";
        case ERROR_ID:
            return "error";
        default:
            return "unknown";
    };
    return "unknown";
}

string HDC::get_type_str(const std::string& path) {
    return get(path).get_type_str();
}

string HDC::get_datashape_str(const std::string& path) {
    return get(path).get_datashape_str();
}

string HDC::get_datashape_str() {
    // TODO: redo this
    throw HDCException("get_datashape_str() is not implemented yet.");
}

int HDC::get_ndim() {
    memcpy(&header,storage->get(uuid),sizeof(header_t)); // TODO: do this in more sophisticately
    return header.ndim;
}

size_t* HDC::get_shape() {
    memcpy(&header,storage->get(uuid),sizeof(header_t));
    return header.shape;
}

std::vector<size_t> HDC::get_strides() {
    memcpy(&header,storage->get(uuid),sizeof(header_t));
    std::vector<size_t> strides;
    size_t elem_size = hdc_sizeof(static_cast<TypeID>(header.type));
    size_t last_stride;
    // TODO this is for C-arrays (row-major)
    for (int i = 0; i < header.ndim; ++i)
    {
        if (i == 0)
        {
            last_stride = elem_size;
        } else
        {
            last_stride = header.shape[header.ndim - i] * last_stride;
        }
        strides.insert(strides.begin(), last_stride);
    }

    return strides;
}

int HDC::get_ndim(const std::string& path) {
    //TODO: make more error-proof - add has check -> make it as function???
    memcpy(&header,storage->get(uuid),sizeof(header_t));
    return get(path).get_ndim();
}

size_t* HDC::get_shape(const std::string& path) {
    memcpy(&header,storage->get(uuid),sizeof(header_t));
    return get(path).get_shape();
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
    return bip::managed_external_buffer(bip::open_only, buffer+sizeof(header_t), 0);
}

map_t* HDC::get_children_ptr() {
    if (header.type != HDC_STRUCT && header.type != HDC_LIST) return nullptr;
    char* buffer = storage->get(uuid);
    auto segment = bip::managed_external_buffer(bip::open_only, buffer+sizeof(header_t), header.buffer_size-sizeof(header_t));
    return segment.find<map_t>("d").first;
}

/* Grows underlying storage by given extra size, it does nothing if extra_size <= 0.*/
void HDC::grow(size_t extra_size) {
    cout << "Growing  " << extra_size << endl;
    if (extra_size <= 0) return;
    char* old_buffer = storage->get(uuid);
    memcpy(&header,old_buffer,sizeof(header_t));
    auto new_size = header.data_size + extra_size;
    D(printf("Growing %luB->%luB\n",header.data_size,new_size);)
    char* new_buffer = buffer_grow(old_buffer, extra_size);
    memcpy(&header,new_buffer,sizeof(header_t));
    storage->set(uuid,new_buffer,new_size);
    if (!storage->usesBuffersDirectly()) delete[] old_buffer;
    if (!storage->usesBuffersDirectly()) delete[] new_buffer;
    return;
}


char* HDC::get_data_ptr() {
    if (header.data_size == 0) return nullptr;
    return (char*)(storage->get(uuid)+sizeof(header_t));
}

void HDC::delete_data() {
    auto buffer = storage->get(uuid);
    memcpy(&header,buffer,sizeof(header_t));
    if ((header.type == HDC_LIST || header.type == HDC_STRUCT) && header.data_size > 0) {
        auto segment = bip::managed_external_buffer(bip::open_only,buffer+sizeof(header_t),0);
        map_t* children = segment.find<map_t>("d").first;
        map_t::nth_index<1>::type& ri=children->get<1>();
        for (auto it = ri.begin(); it != ri.end(); ++it) {
            HDC h(storage,it->address.c_str());
            h.delete_data();
        }
    }
    storage->remove(uuid);
    uuid = "";
}
/* grows buffer provided buffer (copies to larger), it does nothing if extra_size <= 0.*/
char* HDC::buffer_grow(char* old_buffer, size_t extra_size) {
    DEBUG_STDOUT("buffer_grow(extra_size = "+to_string(extra_size)+")\n");
    if (extra_size <= 0 || old_buffer == nullptr) return old_buffer;
    //load header
    header_t header;
    memcpy(&header,old_buffer,sizeof(header_t));
    auto new_data_size = header.data_size + extra_size;
    auto new_buffer_size = new_data_size + sizeof(header_t);
    char* new_buffer = new char[new_buffer_size];
    // if there were children, resize the segment
    if ((header.type == HDC_LIST || header.type == HDC_STRUCT) && header.data_size > 0) {
        // try to open old children
        auto old_segment = bip::managed_external_buffer(bip::open_only,old_buffer+sizeof(header_t),0);
        map_t* old_children = old_segment.find<map_t>("d").first;
        // if there are some, copy them
        if (old_children != nullptr) {
            auto new_segment = bip::managed_external_buffer(bip::create_only,new_buffer+sizeof(header_t),new_data_size);
            map_t* new_children = new_segment.construct<map_t>("d")(map_t::ctor_args_list(),new_segment.get_segment_manager());
            map_t::nth_index<1>::type& ri=old_children->get<1>();
            for (auto it = ri.begin(); it != ri.end(); ++it) {
                record rec(it->key.c_str(),it->address.c_str(),new_segment.get_segment_manager());
                new_children->insert(rec);
            }
        } else {
            throw HDCException("buffer_grow(): children not found\n");
        };
    } else {
        // copy old data to new buffer or something like this here, throw warning now
        DEBUG_STDOUT("Warning: buffer size increased, but no data copied!!!\n");
    }
    // finalize header and copy it to the new buffer
    header.data_size = new_data_size;
    header.buffer_size = new_buffer_size;
    memcpy(new_buffer,&header,sizeof(header_t));
    return new_buffer;
}

// "static contructor" from void* HDC
HDC* HDC::new_HDC_from_cpp_ptr(intptr_t cpp_ptr) {
    HDC* tree;
    tree = (HDC*) cpp_ptr;
    return tree;
}

// "static contructor" from hdc_t*
HDC* HDC::new_HDC_from_c_ptr(intptr_t c_ptr) {
    HDC* tree;
    hdc_t* c_wrap = (hdc_t*) c_ptr;
    tree = (HDC*) c_wrap->obj;
    return tree;
}

HDC* HDC::deserialize_HDC_file(const std::string& filename) {
    try {
        std::ifstream t(filename);
        std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
        return deserialize_HDC_string(str);
    }
    catch (ifstream::failure e) {
        cout << "deserialize_HDC_file(): Error reading / opening file." << endl;
        //TODO: what here???
    }
}

HDC* HDC::deserialize_HDC_string(const std::string& str) {
    HDC* tree;
    pt::ptree root;

    stringstream ss;
    ss << str;
    pt::read_json(ss,root);

    string storage_str = root.get<std::string>("storage");
    options = new pt::ptree();
    HDC::set_default_storage_options(storage_str);
    auto storage_options = root.get_child("settings");
    for (const auto& kv : storage_options) {
        options->add_child("storage-options/"+kv.first,kv.second);
    }
    HDC::search_plugins();
    //HDC::load_config();
    HDC::set_storage(storage_str);
    string uuid = root.get<std::string>("uuid");

    tree = new HDC(global_storage,uuid);
    return tree;
}
