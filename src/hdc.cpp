#include "hdc.hpp"
#include <fstream>
#include <memory>
#include <glob.h>
#include <dlfcn.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/type.hpp>
#include <CLI/CLI.hpp>
//#define DEBUG

using namespace std;

HDCStorage* global_storage = nullptr;

unordered_map<string, string> avail_stores;

pt::ptree* options;

CLI::App app; // This is for command line parsing

void HDC::parse_cmdline(int argc, const char* argv[])
{
    bool _list_plugins = false;
    app.add_flag("--list-plugins", _list_plugins, "lists available storage plugins");
    std::string _storage = "umap";
    app.add_option("--storage", _storage, "storage name (e.g. \"umap\", or \"mdbm\")");
//     app.set_config("--config"); // TODO
    try {
        app.parse(argc, argv);

    } catch (const CLI::ParseError &e) {
        exit(app.exit(e));
    }
    if (_list_plugins) {
        HDC::search_plugins();
        HDC::list_plugins();
        exit(0);
    }
    options->put("storage_cmdline", _storage);
}

void HDC::load_config(std::string configPath)
{
    std::string delimiters(":");
    std::vector<std::string> parts;
    boost::trim_if(configPath, boost::is_any_of(delimiters));
    boost::split(parts, configPath, boost::is_any_of(delimiters), boost::token_compress_on);
    pt::ptree settings_read;
    bool config_found = false;
    for (auto path : parts) {
        if (fileExists(path)) {
            try {
                pt::read_json(path, settings_read);
                config_found = true;
                break;
            }
            catch (std::exception &e) {
                std::cout << e.what() << std::endl;
                std::cout << "HDC::load_config(): something bad happened" << endl;
                exit(1);
            }
        }
    }
    if (config_found) {
        options->put("storage", settings_read.get<std::string>("storage", "umap"));
        boost::optional<pt::ptree&> s = settings_read.get_child_optional("storage_options");
        if (s.is_initialized()) {
            options->add_child("storage_options", *s);
        }
//         else
//             options->add_child("storage_options",pt::ptree());
    }
}

void HDC::search_plugins(std::string searchPath)
{
    std::string delimiters(":");
    std::vector<std::string> parts;
    boost::trim_if(searchPath, boost::is_any_of(delimiters));
    boost::split(parts, searchPath, boost::is_any_of(delimiters), boost::token_compress_on);
    std::string lib_dir = HDC::get_library_dir();
    parts.push_back(lib_dir);
    parts.push_back(lib_dir + "/plugins");
    parts.push_back(lib_dir + "/hdc");
    glob_t globbuf;
    for (auto path : parts) {
        //cout << path << " " << endl;
        string tmp_pth = path + "/libhdc_plugin_*.so*";
        int err = glob(tmp_pth.c_str(), 0, NULL, &globbuf);
        if (err == 0) {
            for (size_t i = 0; i < globbuf.gl_pathc; i++) {
                string pluginPath = globbuf.gl_pathv[i];
                //cout << pluginPath << endl;
                // try to load the plugin and if it runs if succeeds, bring it into hdc_map_t
                try {
                    HDCStorage stor(globbuf.gl_pathv[i], "{\"do_not_init\": true}");
                    string name = stor.name();
                    if (!name.empty()) {
                        if (avail_stores.find(name) == avail_stores.end()) {
                            avail_stores[name] = pluginPath;
                        }
                    }
                } catch (...) {
                    std::cout << "Nope\n";
                }
            }
            globfree(&globbuf);
        }
    }
};

std::vector<std::string> HDC::get_available_plugins()
{
    std::vector<std::string> keys;
    for (auto kv : avail_stores) {
        keys.push_back(kv.first);
    }
    return keys;
}

void HDC::list_plugins()
{
    std::cout << "Available storage plugins:\n";
    for (const auto& store : avail_stores) {
        std::cout << "  - " << store.first << " : " << store.second << "\n";
    }
}

void HDC::set_storage(std::string storage)
{
    boost::optional<std::string> storage_cmd = options->get_optional<std::string>("storage_cmdline");
    if (storage_cmd) {
        while (options->count("storage") > 0) options->erase("storage");
        options->put("storage", *storage_cmd);
        while (options->count("storage_cmdline") > 0) options->erase("storage_cmdline");
    }
    std::string selected_store_name = options->get<std::string>("storage", storage);
    //cout << "Selected storage: " << selected_store_name <<endl;
    if (avail_stores.find(selected_store_name) != avail_stores.end()) {
        //cout << avail_stores[selected_store_name] << endl;
        if (!options->count("storage_options")) options->add_child("storage_options", pt::ptree());
        global_storage = new HDCStorage(avail_stores[selected_store_name], options->get_child("storage_options"));
    } else {
        throw HDCException("Unable to select the store.\n");
    }
}

void HDC::set_default_storage_options(const std::string& storage, const std::string& storage_options)
{
    options->put("storage", storage);
    if (!storage_options.empty()) {
        std::stringstream ss(storage_options);
        pt::ptree* parsed_stor_opt = new pt::ptree();
        pt::read_json(ss, *parsed_stor_opt);
        options->add_child("storage_options", *parsed_stor_opt);
    }
}

std::string HDC::get_library_dir(void)
{
    Dl_info dl_info;
    dladdr((void*)HDC::get_library_dir, &dl_info);
    std::string path = dl_info.dli_fname;
    size_t found=path.find_last_of("/\\");
    return path.substr(0,found);
}

void HDC::init(const std::string& storage_str, const std::string& storage_options)
{
    options = new pt::ptree();
    HDC::set_default_storage_options(storage_str, storage_options);
    HDC::search_plugins();
    //HDC::load_config();
    HDC::set_storage(storage_str);
}


/** Cleans up global_storage  -- mainly due to C and Fortran */
void HDC::destroy()
{
    if (global_storage == nullptr) return;
    delete global_storage;
    global_storage = nullptr;
    delete options;
    options = nullptr;
    DEBUG_STDERR("HDC::destroy(): HDC storage destroyed.\n");
}

//---------------------------- HDC class -----------------------------------

/** Creates empty HDC with specified buffer size */
HDC::HDC(size_t data_size)
{
    // fill some data
    hdc_header_t header;
    memset(&header, 0, sizeof(hdc_header_t));
    header.buffer_size = data_size + sizeof(hdc_header_t);
    header.data_size = data_size;
    header.rank = 1;

    if (global_storage == nullptr) {
        //HDC::init("./plugins/libMDBMPlugin.so","./plugins/settings.txt");
        HDC::init();
        atexit(HDC::destroy);
    }

    // Start by creating segment
    std::vector<char> buffer(header.buffer_size);

    // copy header there -- we need that, hopefully it will be optimized out
    memcpy(buffer.data(), &header, sizeof(hdc_header_t));

    //Store to some storage
    uuid = generate_uuid_str();
    storage = global_storage;
    storage->set(uuid, buffer.data(), header.buffer_size);
}


/** Default constructor. Creates empty HDC */
HDC::HDC() : HDC(0lu)
{};

/** Creates empty HDC with specified type and shape */
HDC::HDC(std::vector<size_t>& shape, hdc_type_t type, long flags)
{
    auto rank = shape.size();
    hdc_header_t header;
    if (rank >= HDC_MAX_DIMS) {
        throw HDCException("HDC(): Unsupported number of dimensions: " + to_string(rank));
    }
    size_t elem_size = 1;
    memset(&header, 0, sizeof(hdc_header_t));
    for (size_t i = 0; i < rank; i++) {
        header.shape[i] = shape[i];
        elem_size *= shape[i];
    }
    header.type = type;
    header.flags = flags;
    header.rank = rank;
    header.data_size = elem_size * hdc_sizeof(type);
    header.buffer_size = header.data_size + sizeof(hdc_header_t);
    std::vector<char> buffer(header.buffer_size);
    memcpy(buffer.data(), &header, sizeof(hdc_header_t));
    uuid = generate_uuid_str();
    storage = global_storage;
    storage->set(uuid, buffer.data(), header.buffer_size);
}

/** Creates a new HDC instance from a given string. If a supplied string contains uri, it tries to open a given resource */
HDC::HDC(const std::string str) : HDC()
{
    this->set_string(str);
}

/** Copy constructor */
HDC::HDC(const HDC& h)
{
    storage = h.storage;
    uuid = h.uuid;
};

/** Deserializing constructor */
HDC::HDC(HDCStorage* _storage, const std::string& _uuid)
{
    uuid = _uuid;
    storage = _storage;
}

HDC::HDC(hdc_t& obj) {
    storage = (HDCStorage*)obj.storage;
    uuid = obj.uuid;
}

/** Destructor */
HDC::~HDC()
{
    /*
    auto t = get_type();
    if (!is_terminal()) {
        try {
            hdc_map_t* children = get_children_ptr();
            for (hdc_map_t::iterator it = children->begin(); it != children->end(); ++it) {
                storage->remove(it->address.c_str());
            }
        } catch(...) {
            return;
        }
    }
    */
    //storage->remove(uuid); // This is responsibility of storage from now
}

//---------------------------- Header information ----------------------------------
size_t HDC::get_datasize() const
{
    return get_header().data_size;
}

size_t HDC::get_size() const
{
    return get_header().buffer_size;
}

/** Returns type of current node. */
size_t HDC::get_type() const
{
    return get_header().type;
}

/** Returns the size of a single item in bytes */
size_t HDC::get_itemsize() const
{
    return hdc_sizeof(static_cast<hdc_type_t>(get_header().type));
}

size_t HDC::get_flags() const
{
    return get_header().flags;
}

template <typename T>
T* HDC::get_data() const
{
    return reinterpret_cast<T*>(get_buffer() + sizeof(hdc_header_t));
}

bool HDC::is_external() const
{
    return (get_header().flags & HDCExternal) != 0;
}

bool HDC::is_readonly() const
{
    return (get_header().flags & HDCReadOnly) != 0;
}

bool HDC::is_fortranorder() const
{
    return (get_header().flags & HDCFortranOrder) != 0;
}

void HDC::print_info() const
{
    hdc_header_t header = get_header();
    printf("Size:\t\t%zu\n", header.buffer_size);
    printf("NDim:\t\t%zu\n", header.rank);
    printf("Shape:\t\t");
    for (size_t i = 0; i < HDC_MAX_DIMS; i++) printf("%zu,", header.shape[i]);
    printf("\n");
    printf("Data Size:\t\t%zu\n", header.data_size);
    printf("External:\t\t%d\n", is_external());
    printf("ReadOnly:\t\t%d\n", is_readonly());
    printf("FortranOrder:\t%d\n", is_fortranorder());
    std::cout << "Type:\t" << get_type_str() << "\n";
    return;
}

//---------------------------- Tree manipulation -----------------------------------

bool HDC::exists(const std::string& path) const
{
    auto pth = split(path);
    return exists(pth);
}

bool HDC::exists_single(hdc_index_t index) const
{
    auto children = get_children_ptr();
    if (children == nullptr) return false;
    if (index.type() == typeid(size_t)) {
        return (children->size() > boost::get<size_t>(index));
    } else {
        return (bool)children->count(boost::get<std::string>(index).c_str());
    }
}

bool HDC::exists(size_t index) const
{
    return exists_single(index);
}

bool HDC::exists(hdc_path_t& path) const
{
    D(
        std::cout << "exists(";
        for {auto str : path} std::cout << str;
        std::cout << ")\n";
    )
    if (path.empty()) {
        return false; //TODO: re-do this!!!
    }
    if (is_terminal()) {
        return false;
    }
    auto first = path.front();
    path.pop_front();

    auto children = get_children_ptr();
    if (children == nullptr) return false;
    if (first.type() == typeid(std::string)) {
        if (children->count(boost::get<std::string>(first).c_str()) == 0) return false;
    }
    if (path.empty()) {
        return exists_single(first);
    } else {
        if (first.type() == typeid(size_t)) {
            try {
                size_t index = boost::get<size_t>(first);
                if (children->size() <= index) return false;
                auto it = children->get<by_index>()[index];
                HDC child(storage, it.address.c_str());
                return child.exists(path);
            } catch (std::exception& e) {
                std::cerr << "exists(): Caught exception: index" << "\n";
                std::cerr << e.what() << std::endl;
                return false;
            }
        } else {
            try {
                auto it = children->find(boost::get<std::string>(first).c_str());
                if (it != children->end()) {
                    HDC ch(storage, it->address.c_str());
                    return ch.exists(path);
                } else { return false; } // TODO Create error HDC obj here???
            } catch (...) {
                std::cerr << "exists(): Caught exception: string" << "\n";
                return false;
            }
        }
    }
    return false; // never goes here
}

void HDC::add_child(hdc_path_t& path, HDC& n)
{
    auto first = path.front();
    path.pop_front();

    if (!path.empty()) { // Create intermediate nodes here
        HDC h;
        if (first.type() == typeid(size_t)) {
            auto index = boost::get<size_t>(first);
            if (!exists_single(index)) insert(index, h);
        } else {
            add_child_single(boost::get<std::string>(first), h);
        }
        get_single(first).add_child(path, n);
    } else {
        if (first.type() == typeid(size_t)) {
            auto index = boost::get<size_t>(first);
            if (!exists_single(index)) insert(index, n);
        } else {
            add_child_single(boost::get<std::string>(first), n);
        }
    }
    return;
}

void HDC::add_child_single(const std::string& path, HDC& n)
{
    D(
        std::cout << "add_child_single(" + path + ")\n";
    )
    // sync buffer
    hdc_header_t header;
    auto buffer = storage->get(uuid);
    memcpy(&header, buffer, sizeof(hdc_header_t));
    size_t old_size = header.buffer_size;
    if (!(header.type == HDC_EMPTY || header.type == HDC_STRUCT)) {
        throw HDCException("add_child_single(): Cannot add child to this node. Data assigned???\n");
    }
    if (header.type == HDC_EMPTY) set_type(HDC_STRUCT);

    if (path.size() > 1024) {
        throw HDCException("add_child_single(): string too long.\n");
    }

    // load new buffer
    buffer = storage->get(uuid);
    memcpy(&header, buffer, sizeof(hdc_header_t));
    vector<char> new_buffer;
    auto segment = get_segment();
    auto children = segment.find<hdc_map_t>("d").first;
    if (children == nullptr) throw HDCException("add_child_single(): Could not get the children.\n");
    if (children->count(path.c_str()) == 0) {
        // Try to grow buffer HDC_MAX_RESIZE_ATTEMPTS times, die if it does not help
        int redo = 1;
        for (size_t i = 0; i < HDC_MAX_RESIZE_ATTEMPTS - 1; i++) {
            if (redo == 0) break;
            try {
                // Storing record larger than 114 bytes can mess up the heap on small buffers (1300b, key with longer than 24 characters), so we need to increase the buffer size here
                if (segment.get_free_memory() < 4 * path.size()) {
                    throw (HDCBadAllocException()); // There can be problem to store large strings
                }
                record rec(path.c_str(), n.get_uuid().c_str(), segment.get_segment_manager());
                try {
                    children->insert(rec);
                }
                catch (boost::interprocess::bad_alloc& e) {
                    throw (HDCBadAllocException());
                }
                redo = 0;
            }
            catch (HDCBadAllocException& e) {
               new_buffer = buffer_grow(buffer, max(header.buffer_size, 4 * path.size()));
               if (new_buffer.data() == buffer) {
                   throw HDCException("grow called, but buffer == new_buffer.\n");
               }
               //if (!storage->usesBuffersDirectly()) delete[] buffer;
               storage->remove(uuid);
               buffer = new_buffer.data();
               memcpy(&header, buffer, sizeof(hdc_header_t));
               segment = bip::managed_external_buffer(bip::open_only, buffer + sizeof(hdc_header_t), 0);
               children = segment.find<hdc_map_t>("d").first;
               redo = 1;
            }
            if (redo == 1 && i == HDC_MAX_RESIZE_ATTEMPTS - 1) {
                throw HDCBadAllocException("add_child_single(): Could not allocate enough memory.\n");
            }
        }
        header.shape[0] = children->size();
        memcpy(buffer, &header, sizeof(hdc_header_t));
        if (header.buffer_size != old_size) {
            storage->set(uuid, buffer, header.buffer_size);
        }
    }
    return;
}

std::vector<std::string> HDC::keys() const
{
    std::vector<std::string> k;
    hdc_map_t* children;
    children = get_children_ptr();
    if (children == nullptr) return k;
    k.reserve(children->size());
    auto& ri = children->get<by_index>();
    for (auto it = ri.begin(); it != ri.end(); ++it) {
        k.push_back(it->key.c_str());
    }
    return k;
}

void HDC::add_child(const std::string& path, HDC& n)
{
    DEBUG_STDOUT("add_child(" + path + ")\n");
    if (path.empty()) throw HDCException("HDC::add_child(): empty path.");
    auto pth = split(path);
    add_child(pth, n);
    return;
}

void HDC::clean() {
    if (!is_terminal()) {
        try {
            hdc_map_t* children = get_children_ptr();
            if (children == nullptr) return;
            for (hdc_map_t::iterator it = children->begin(); it != children->end(); ++it) {
                //HDC(this->storage,it->key.c_str()).clean();
                storage->remove(it->address.c_str());
            }
        } catch(...) {
            return;
        }
    }
    storage->remove(uuid); // This is responsibility of storage from now
}

void HDC::delete_child(hdc_path_t& path)
{
    hdc_header_t header = get_header();
    D(
        std::cout << "delete_child(";
        for {auto str: path} std::cout << str;
        std::cout << ")\n";
    )
    auto path2 = path;
    if (!exists(path2) || path.empty()) {
        return;
    }
    auto first = path.front();
    path.pop_front();
    hdc_map_t* children = get_children_ptr();
    auto buffer = get_buffer();
    if (path.empty()) {
        //size_t index;
        if (first.type() == typeid(size_t)) {
            auto it = children->get<by_index>()[boost::get<size_t>(first)];
            storage->remove(it.address.c_str());
            // Maybe this should not be supported...
        } else {
            auto it = children->find(boost::get<std::string>(first).c_str());
            if (it != children->end()) {
                storage->remove(it->address.c_str());
                children->erase(it);
            }
        }
        header.shape[0] = children->size();
        memcpy(buffer, &header, sizeof(hdc_header_t));
    } else {
        get(boost::get<std::string>(first)).delete_child(path);
    }
    // set type back to empty if the only child was deleted.
    //if (children->empty()) set_type(HDC_EMPTY); Not sure if to do this
    return;
}

void HDC::delete_child(const std::string& path)
{
    auto pth = split(path);
    delete_child(pth);
    return;
}

HDC HDC::get(hdc_path_t& path)
{
    D(
        std::cout << "get(";
        for {auto str: path} std::cout << str;
        std::cout << ")\n";
    )
    // Return itself when empty list
    if (path.empty()) {
        return *this;
    }
    // else do lookup
    auto first = path.front();
    path.pop_front();
    hdc_map_t* children = get_children_ptr();

    if (children == nullptr) {
        throw HDCException("get(): This node has no children.");
    }

    return get_single(first).get(path);
}

const HDC HDC::get(hdc_path_t& path) const
{
    D(
        std::cout << "get(";
        for {auto str: path} std::cout << str;
        std::cout << ")\n";
    )
    // Return itself when empty list
    if (path.empty()) {
        return *this;
    }
    // else do lookup
    auto first = path.front();
    path.pop_front();
    hdc_map_t* children = get_children_ptr();

    if (children == nullptr) {
        throw HDCException("get(): This node has no children.");
    }

    return get_single(first).get(path);
}

HDC HDC::get_single(hdc_index_t index)
{
    DEBUG_STDOUT("get_single(" + to_string(index) + ")\n");

    hdc_map_t* children = get_children_ptr();

    if (index.type() == typeid(std::string)) {
        auto str = boost::get<std::string>(index).c_str();
        if (strlen(str) == 0) return *this;
        if (children->count(str)) {
            return HDC(storage, children->find(str)->address.c_str());
        } else {
            throw HDCException("get(std::string): Not found\n");
        }
    } else {
        size_t i = boost::get<size_t>(index);
        if (i >= children->size()) {
            throw HDCException("get(index): index > size()\n");
        }
        return HDC(storage, children->get<by_index>()[i].address.c_str());
    }
}

const HDC HDC::get_single(hdc_index_t index) const
{
    DEBUG_STDOUT("get_single(" + to_string(index) + ")\n");

    hdc_map_t* children = get_children_ptr();

    if (index.type() == typeid(std::string)) {
        auto str = boost::get<std::string>(index).c_str();
        if (strlen(str) == 0) return *this;
        if (children->count(str)) {
            return HDC(storage, children->find(str)->address.c_str());
        } else {
            throw HDCException("get(std::string): Not found\n");
        }
    } else {
        size_t i = boost::get<size_t>(index);
        if (i >= children->size()) {
            throw HDCException("get(index): index > size()\n");
        }
        return HDC(storage, children->get<by_index>()[i].address.c_str());
    }
}

HDC HDC::get(const std::string& path)
{
    if (path.empty()) return *this;
    else {
        auto pth = split(path);
        return get(pth);
    }
}

const HDC HDC::get(const std::string& path) const
{
    if (path.empty()) return *this;
    else {
        auto pth = split(path);
        return get(pth);
    }
}

HDC HDC::get(size_t index)
{
    return get_single(index);
}

const HDC HDC::get(size_t index) const
{

    return get_single(index);
}


HDC HDC::get_or_create(hdc_path_t& path)
{
    if (path.empty()) return *this;
    if (!exists(path)) {
        HDC h;
        add_child(path,h);
        return h;
    } else {
        return get(path);
    }
}

HDC HDC::get_or_create(const std::string& path)
{
    if (path.empty()) return *this;
    if (!exists(path)) {
        HDC h;
        add_child(path,h);
        return h;
    } else {
        return get(path);
    }
}

HDC HDC::get_or_create(size_t index)
{
    if (!exists(index)) {
        HDC h;
        insert(index,h);
        return h;
    } else {
        return get(index);
    }
}

HDC& HDC::operator=(const HDC& other)
{
    if (this != &other && uuid != other.get_uuid()) {
        //remove unused (orphaned) buffer here?
    }
    uuid = other.get_uuid();
    storage = other.get_storage();
    return *this;
}

HDC& HDC::operator=(char const* str)
{
    set_string(str);
    return *this;
}

HDC& HDC::operator=(const std::string& str)
{
    set_string(str.c_str());
    return *this;
}

HDC HDC::operator[](const std::string& path)
{
    return get_or_create(path);
}

const HDC HDC::operator[](const std::string& path) const
{
    return get(path);
}


HDC HDC::operator[](size_t index)
{
    return get_or_create(index);
}

const HDC HDC::operator[](size_t index) const
{
    return get_single(index);
}

void HDC::set_child_single(hdc_index_t path, HDC& n)
{
    D(
        std::cout << "set_child_single(";
        for {auto str: path} std::cout << str;
        std::cout << ")\n";
    )
    hdc_map_t* children = get_children_ptr();
    auto ca = get_segment().get_allocator<record>();
    if (path.type() == typeid(size_t)) {
        size_t i = boost::get<size_t>(path);
        if (get_type() != HDC_LIST) {
            throw HDCException("set_child_single(): called on non list type node\n");
        }
        if (i >= children->size()) {
            throw HDCException("set_child_single(): Index " + std::to_string(i) + " >= list size.\n");
        }
        shared_string str(n.get_uuid().c_str(), ca);
        children->modify(children->iterator_to(children->get<by_index>()[i]), change_node(str));
    } else {
        // check whether children container exists
        if (children == nullptr) children = get_children_ptr();
        // check whether given path exists
        if (children->count(boost::get<std::string>(path).c_str())) {
            shared_string str(n.get_uuid().c_str(), ca); // TODO wrap this up to try catch block
            children->modify(children->find(boost::get<std::string>(path).c_str()), change_node(str));
        } else {
            // TODO: get_allocator -- viz vyse...
            children->insert(record(boost::get<std::string>(path).c_str(), n.get_uuid().c_str(), ca));
        }
    }
    return;
}

void HDC::set_child(size_t index, HDC& n)
{
    set_child_single(index,n);
}

void HDC::set_child(hdc_path_t& path, HDC& n)
{
    D(
        std::cout << "set_child(";
        for {auto str: path} std::cout << str;
        std::cout << ")\n";
    )
    auto path2 = path;
    if (!exists(path2)) { // Nothing to set
        std::cout << "Nothing to set, maybe you want to add..." << endl;
        return;
    }
    auto first = path.front();
    path.pop_front();
    if (path.empty()) {
        set_child_single(first, n);
    } else {
        get(boost::get<std::string>(first)).set_child(path, n);
    }
    return;
}

void HDC::set_child(const std::string& path, HDC& n)
{
    auto pth = split(path);
    set_child(pth, n);
    return;
}

//------------------ Data manipulation -------------------------------

void HDC::set_type(hdc_type_t type)
{
    // More to be added here later
    DEBUG_STDOUT("set_type(" + to_string(header.type) + " -> " + to_string(type) + ")\n");
    hdc_header_t header;
    char* old_buffer = storage->get(uuid);
    memcpy(&header, old_buffer, sizeof(hdc_header_t)); //sync header
    if (header.type == type) return; // Nothing to do
    header.type = type;
    memcpy(old_buffer, &header, sizeof(hdc_header_t)); //sync header back
    if (!is_terminal()) {
        if (header.data_size < HDC_NODE_SIZE_DEFAULT) {
            vector<char> new_buffer(HDC_NODE_SIZE_DEFAULT);
            header.data_size = HDC_NODE_SIZE_DEFAULT - sizeof(hdc_header_t);
            header.buffer_size = HDC_NODE_SIZE_DEFAULT;
            memcpy(new_buffer.data(), &header, sizeof(hdc_header_t));
            bip::managed_external_buffer segment(bip::create_only, new_buffer.data() + sizeof(hdc_header_t), header.data_size);
            auto children = segment.construct<hdc_map_t>("d")(hdc_map_t::ctor_args_list(),hdc_map_t::allocator_type(segment.get_segment_manager())); // TODO: Wrap this to auto-growing???
            if (children == nullptr) throw HDCException("HDC::set_type(hdc_type_t type): Could not create the children");
            storage->set(uuid, new_buffer.data(), header.buffer_size);
        } else {
            bip::managed_external_buffer segment(bip::create_only, old_buffer + sizeof(hdc_header_t), header.data_size);
            auto children = segment.construct<hdc_map_t>("d")(hdc_map_t::ctor_args_list(),hdc_map_t::allocator_type(segment.get_segment_manager())); // TODO: Wrap this to auto-growing???
            if (children == nullptr) throw HDCException("HDC::set_type(hdc_type_t type): Could not create the children");
            storage->set(uuid, old_buffer, header.buffer_size);
        }
    }
    return;
}

intptr_t HDC::as_void_ptr() const
{
    return (intptr_t)(void*)this;
}

void HDC::dump() const
{
    std::cout << to_json(0) << std::endl;
}

/** Serializes HDC to special json file*/
void HDC::serialize(const std::string& filename) const
{
    ofstream file(filename);
    file << serialize();
    file.close();
}

const std::string HDC::serialize() const
{
    pt::ptree root = this->storage->get_status();
    root.put("uuid", this->uuid);
    std::stringstream ss;
    write_json(ss, root);
    return ss.str();
}

void HDC::set_data_c(std::vector<size_t>& shape, void* data, hdc_type_t type, hdc_flags_t flags)
{
    D(printf("set_data_c(%d, {%d,%d,%d}, %f, %s)\n", rank, shape[0], shape[1], shape[2], ((double*)data)[0],
             hdc_type_str(type).c_str());)
    auto rank = shape.size();
    hdc_header_t header;
    auto buffer = storage->get(uuid);
    memcpy(&header, buffer, sizeof(hdc_header_t));
    // Start with determining of the buffer size
    size_t data_size = hdc_sizeof(type);
    for (size_t i = 0; i < rank; i++) data_size *= shape[i];
    size_t buffer_size = data_size + sizeof(hdc_header_t);
    if (header.buffer_size == buffer_size) {
        storage->lock(uuid);
        memcpy(buffer + sizeof(hdc_header_t), data, data_size);
        storage->unlock(uuid);
    } else {
        header.buffer_size = buffer_size;
        header.data_size = data_size;
        header.flags = flags;
        memset(header.shape, 0, HDC_MAX_DIMS * sizeof(size_t));
        for (size_t i = 0; i < rank; i++) header.shape[i] = shape[i];
        header.type = type;
        header.rank = rank;
        std::vector<char> buffer(header.buffer_size);
        memcpy(buffer.data(), &header, sizeof(hdc_header_t));
        memcpy(buffer.data() + sizeof(hdc_header_t), data, header.data_size);
        storage->set(uuid, buffer.data(), header.buffer_size);
    }
    return;
}


void HDC::set_data_c(std::vector<size_t>& shape, const void* data, hdc_type_t type, hdc_flags_t flags)
{
    D(printf("set_data_c(%d, {%d,%d,%d}, %f, %s)\n", rank, shape[0], shape[1], shape[2], ((double*)data)[0],
             hdc_type_str(type).c_str());)
    auto rank = shape.size();
    hdc_header_t header;
    auto buffer = storage->get(uuid);
    memcpy(&header, buffer, sizeof(hdc_header_t));
    // Start with determining of the buffer size
    size_t data_size = hdc_sizeof(type);
    for (size_t i = 0; i < rank; i++) data_size *= shape[i];
    size_t buffer_size = data_size + sizeof(hdc_header_t);
    if (header.buffer_size == buffer_size) {
        storage->lock(uuid);
        memcpy(buffer + sizeof(hdc_header_t), data, data_size);
        storage->unlock(uuid);
        return;
    } else {
        header.buffer_size = buffer_size;
        header.data_size = data_size;
        header.flags = flags;
        memset(header.shape, 0, HDC_MAX_DIMS * sizeof(size_t));
        for (size_t i = 0; i < rank; i++) header.shape[i] = shape[i];
        header.type = type;
        header.rank = rank;
        std::vector<char> buffer(header.buffer_size);
        memcpy(buffer.data(), &header, sizeof(hdc_header_t));
        memcpy(buffer.data() + sizeof(hdc_header_t), data, header.data_size);
        storage->set(uuid, buffer.data(), header.buffer_size);
        return;
    }
}

hdc_t HDC::as_obj() {
    hdc_t res;
    res.storage = storage;
    strcpy(res.uuid,uuid.c_str());
    return res;
}

void HDC::insert(size_t index, HDC& h)
{
    DEBUG_STDOUT("insert(" + to_string(i) + ")\n");
    //sync buffer
    hdc_header_t header;
    auto buffer = storage->get(uuid);
    memcpy(&header, buffer, sizeof(hdc_header_t));
    size_t old_size = header.buffer_size;
    if (header.type != HDC_EMPTY && header.type != HDC_LIST) {
        throw HDCException("insert(): Cannot add child to this node. Data assigned???\n");
    }
    if (header.type == HDC_EMPTY) set_type(HDC_LIST);
    buffer = storage->get(uuid);
    memcpy(&header, buffer, sizeof(hdc_header_t));
    if (get_shape()[0] < index) {
        std::cout << "Warning: insert():: inserting behind current list length, filling with empty containers\n";
        for (size_t i = header.shape[0]; i < index; i++) {
            HDC ch;
            append(ch);
        }
    }

    // load new buffer
    buffer = storage->get(uuid);
    memcpy(&header, buffer, sizeof(hdc_header_t));
    vector<char> new_buffer;
    auto segment = get_segment();
    auto children = segment.find<hdc_map_t>("d").first;
    if (children == nullptr) throw HDCException("insert(): Could not get the children.\n");

// Try to grow buffer HDC_MAX_RESIZE_ATTEMPTS times, die if it does not help
    int redo = 1;
    for (size_t i = 0; i < HDC_MAX_RESIZE_ATTEMPTS - 1; i++) {
        if (redo == 0) break;
        try {
            hdc_map_t::nth_index<1>::type& ri = children->get<1>();
            record rec(to_string(index).c_str(), h.get_uuid().c_str(), segment.get_segment_manager());
            try {
                ri.insert(ri.begin() + index, rec);
            }
            catch (boost::interprocess::bad_alloc& e) {
                throw (HDCBadAllocException());
            }
            redo = 0;
        }
        catch (HDCBadAllocException& e) {
            new_buffer = buffer_grow(buffer, header.buffer_size);
            if (new_buffer.data() == buffer) {
                throw HDCException("grow called, but buffer == new_buffer.\n");
            }
            //if (!storage->usesBuffersDirectly()) delete[] buffer;
            storage->remove(uuid);
            buffer = new_buffer.data();
            memcpy(&header, buffer, sizeof(hdc_header_t));
            segment = bip::managed_external_buffer(bip::open_only, buffer + sizeof(hdc_header_t), 0);
            children = segment.find<hdc_map_t>("d").first;
            redo = 1;
        }
        if (redo == 1 && i == HDC_MAX_RESIZE_ATTEMPTS - 1) {
            throw HDCBadAllocException("insert(): Could not allocate enough memory.\n");
        }
    }
    header.shape[0] = children->size();
    memcpy(buffer, &header, sizeof(hdc_header_t));
    if (header.buffer_size != old_size) {
        storage->set(uuid, buffer, header.buffer_size);
    }
    return;
}

void HDC::append(HDC& h)
{
    insert(get_header().shape[0], h);
    return;
}

const char* HDC::get_type_str() const
{
    switch (get_type()) {
        case HDC_EMPTY:
            return "null";
        case HDC_STRUCT:
            return "struct";
        case HDC_LIST:
            return "list";
        case HDC_INT8:
            return "int8";
        case HDC_INT16:
            return "int16";
        case HDC_INT32:
            return "int32";
        case HDC_INT64:
            return "int64";
        case HDC_UINT8:
            return "int8";
        case HDC_UINT16:
            return "int16";
        case HDC_UINT32:
            return "int32";
        case HDC_UINT64:
            return "int64";
        case HDC_FLOAT:
            return "float32";
        case HDC_DOUBLE:
            return "float64";
        case HDC_STRING:
            return "string";
        case HDC_BOOL:
            return "bool";
        case HDC_ERROR:
            return "error";
        default:
            return "unknown";
    };
    return "unknown";
}

hdc_header_t HDC::get_header() const {
    hdc_header_t h;
    memcpy(&h,storage->get(uuid),sizeof(hdc_header_t));
    return h;
}

std::vector<size_t> HDC::get_shape() const
{
    hdc_header_t header = get_header();
//     size_t offset = reinterpret_cast<size_t>(header.shape) - reinterpret_cast<size_t>(&header);
    std::vector<size_t> shape(&header.shape[0], &header.shape[0]+header.rank);
    //TODO: C++17 has offsetoff
//     return reinterpret_cast<size_t*>(get_buffer() + offset);
    return shape;
}

std::vector<size_t> HDC::get_strides() const
{
    hdc_header_t header = get_header();
    std::vector<size_t> strides;
    size_t elem_size = hdc_sizeof(static_cast<hdc_type_t>(header.type));
    size_t last_stride;
    if (is_fortranorder()) {
        for (size_t i = 0; i < header.rank; ++i) {
            if (i == 0) {
                last_stride = elem_size;
            } else {
                last_stride = header.shape[i - 1] * last_stride;
            }
            strides.insert(strides.end(), last_stride);
        }
    } else {
        for (size_t i = 0; i < header.rank; ++i) {
            if (i == 0) {
                last_stride = elem_size;
            } else {
                last_stride = header.shape[header.rank - i] * last_stride;
            }
            strides.insert(strides.begin(), last_stride);
        }
    }
    return strides;
}

size_t HDC::get_rank() const
{
    return get_header().rank;
}

size_t HDC::children_count() const
{
    auto children = get_children_ptr();
    if (children != nullptr)
        return get_children_ptr()->size();
    else return 0;
}

char* HDC::get_buffer() const
{
    return storage->get(uuid);
}

string HDC::get_uuid() const
{
    return uuid;
}

// allocator stuff
bip::managed_external_buffer HDC::get_segment() const
{
    char* buffer = storage->get(uuid);
    bip::managed_external_buffer segment;
    return bip::managed_external_buffer(bip::open_only, buffer + sizeof(hdc_header_t), 0);
}

hdc_map_t* HDC::get_children_ptr() const
{
    if (is_terminal()) return nullptr;
    auto segment = get_segment();
    return segment.find<hdc_map_t>("d").first;
}

const std::map<std::string,HDC> HDC::get_children() const
{
    auto children = get_children_ptr();
    std::map<std::string,HDC> ch;
    if (children != nullptr) {
        for (auto it = children->begin(); it != children->end(); ++it) {
            ch[it->key.c_str()] = HDC(storage,it->address.c_str());
        }
    }
    return ch;
}

const std::vector<HDC> HDC::get_slices() const
{
    auto children = get_children_ptr();
    std::vector<HDC> ch;
    if (children != nullptr) {
        ch.reserve(children->size());
        for (auto it = children->begin(); it != children->end(); ++it) {
            ch.push_back(HDC(storage,it->address.c_str()));
        }
    }
    return ch;
}

/* Grows underlying storage by given extra size, it does nothing if extra_size <= 0.*/
void HDC::grow(size_t extra_size)
{
    hdc_header_t header = get_header();
    if (extra_size <= 0) return;
    char* old_buffer = storage->get(uuid);
    memcpy(&header, old_buffer, sizeof(hdc_header_t));
    auto new_size = header.data_size + extra_size;
    D(printf("Growing %luB->%luB\n", header.data_size, new_size);)
    vector<char> new_buffer = buffer_grow(old_buffer, extra_size);
    memcpy(&header, new_buffer.data(), sizeof(hdc_header_t));
    storage->set(uuid, new_buffer.data(), new_size);
//     if (!storage->usesBuffersDirectly()) delete[] old_buffer;
//     if (!storage->usesBuffersDirectly()) delete[] new_buffer;
    return;
}


char* HDC::get_data_ptr() const
{
    hdc_header_t header = get_header();
    if (header.data_size == 0) return nullptr;
    return (char*)(storage->get(uuid) + sizeof(hdc_header_t));
}

/* grows buffer provided buffer (copies to larger), it does nothing if extra_size <= 0.*/
std::vector<char> HDC::buffer_grow(char* old_buffer, size_t extra_size)
{
    DEBUG_STDOUT("buffer_grow(extra_size = " + to_string(extra_size) + ")\n");
    //load header
    hdc_header_t header;
    memcpy(&header, old_buffer, sizeof(hdc_header_t));
    if (old_buffer == nullptr) return vector<char>(0);
    if (extra_size <= 0) return vector<char>(old_buffer,old_buffer+header.buffer_size);
    auto newdata_size = header.data_size + extra_size;
    auto new_buffer_size = newdata_size + sizeof(hdc_header_t);
    std::vector<char> new_buffer(new_buffer_size);
//     auto new_buffer = new std::vector<char>(new_buffer_size);
    // if there were children, resize the segment
    if ((header.type == HDC_LIST || header.type == HDC_STRUCT) && header.data_size > 0) {
        // try to open old children
        auto old_segment = bip::managed_external_buffer(bip::open_only, old_buffer + sizeof(hdc_header_t), 0);
        hdc_map_t* old_children = old_segment.find<hdc_map_t>("d").first;
        // if there are some, copy them
        if (old_children != nullptr) {
            auto new_segment = bip::managed_external_buffer(bip::create_only, new_buffer.data() + sizeof(hdc_header_t),
                                                            newdata_size);
            hdc_map_t* new_children = new_segment.construct<hdc_map_t>("d")(hdc_map_t::ctor_args_list(),
                                                                    new_segment.get_segment_manager());
            hdc_map_t::nth_index<1>::type& ri = old_children->get<1>();
            for (auto it = ri.begin(); it != ri.end(); ++it) {
                record rec(it->key.c_str(), it->address.c_str(), new_segment.get_segment_manager());
                new_children->insert(rec);
            }
        } else {
            throw HDCException("buffer_grow(): children not found\n");
        };
    } else {
        memcpy(new_buffer.data()+sizeof(hdc_header_t), old_buffer+sizeof(hdc_header_t), sizeof(hdc_header_t));
    }
    // finalize header and copy it to the new buffer
    header.data_size = newdata_size;
    header.buffer_size = new_buffer_size;
    memcpy(new_buffer.data(), &header, sizeof(hdc_header_t));
    return new_buffer;
}

HDC HDC::deserialize_HDC_file(const std::string& filename)
{
    try {
        std::ifstream t(filename);
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        return deserialize_HDC_string(str);
    }
    catch (const ifstream::failure& e) {
        std::cout << "deserialize_HDC_file(): Error reading / opening file." << endl;
        return HDC();
    }
}

HDC HDC::deserialize_HDC_string(const std::string& str)
{
    HDC tree;
    pt::ptree root;

    stringstream ss;
    ss << str;
    pt::read_json(ss, root);

    string storage_str = root.get<std::string>("storage");
    options = new pt::ptree();
    HDC::set_default_storage_options(storage_str,"");
    auto storage_options = root.get_child("settings");
    for (const auto& kv : storage_options) {
        options->add_child("storage-options/" + kv.first, kv.second);
    }
    HDC::search_plugins();
    //HDC::load_config();
    HDC::set_storage(storage_str);
    string uuid = root.get<std::string>("uuid");

    return HDC(global_storage, uuid);
}


/** Creates a new HDC instance from a given string. If a supplied string contains uri, it tries to open a given resource */
HDC HDC::load(const std::string& uri, const std::string& datapath)
{
    HDC h;
    // start by parsing the string
    std::vector<std::string> result;
    boost::algorithm::split_regex(result, uri, boost::regex("://"));
    if (result.size() > 1) {
        std::vector<std::string> split_res;
        boost::split(split_res, result[1], boost::is_any_of("|"), boost::token_compress_on);
        if (split_res.size() > 1 && datapath != "") {
            throw HDCException("Both second argument and | in path specified. Use just one of them.");
        }
        if (split_res.size() == 1) split_res.push_back("");
        if (!datapath.empty()) split_res[1] = datapath;
        auto prefix = result[0];
        if (prefix == "hdf5") {
            return from_hdf5(split_res[0], split_res[1]);
        } else if (prefix == "json") {
            return from_json(split_res[0], split_res[1]);
        } else if (prefix == "uda") {
            return from_uda(split_res[0], split_res[1]);
        } else if (prefix == "uda_new") {
            return uda2HDC(split_res[0], split_res[1]);
        } else {
            throw HDCException("Protocol " + prefix + " not known\n");
        }
    } else {
        throw HDCException("Missing protocol, The URI should look like: protocol://address|optional arguments\n");
    }
    return h;
}

hdc_data_t HDC::get_data() const
{
        hdc_header_t header;
        auto buffer = storage->get(uuid);
        memcpy(&header,buffer,sizeof(hdc_header_t));
        hdc_data_t obj;
        obj.type = header.type;
        obj.flags = header.flags;
        obj.rank = header.rank;
        for (size_t i=0;i<HDC_MAX_DIMS;i++) obj.shape[i] = header.shape[i];
        obj.data = buffer+sizeof(hdc_header_t);
        return obj;
}

void HDC::set_data(hdc_data_t obj)
{
        hdc_header_t header;
        auto buffer = storage->get(uuid);
        memcpy(&header,buffer,sizeof(hdc_header_t));
        // Start with determining of the buffer size
        size_t data_size = hdc_sizeof(obj.type);
        for (size_t i = 0; i < obj.rank; i++) data_size *= obj.shape[i];
        size_t buffer_size = data_size + sizeof(hdc_header_t);
        if (header.buffer_size == buffer_size) {
            storage->lock(uuid);
            header.type = obj.type;
            header.flags = obj.flags;
            memcpy(buffer,&header,sizeof(hdc_header_t));
            memcpy(buffer + sizeof(hdc_header_t), obj.data, data_size);
            storage->unlock(uuid);
            return;
        } else {
            storage->remove(uuid);
            header.type = obj.type;
            header.flags = obj.flags;
            header.rank = obj.rank;
            memset(header.shape,0,HDC_MAX_DIMS*sizeof(size_t));
            header.data_size = hdc_sizeof(obj.type);
            for (size_t i=0;i<header.rank;i++) {
                header.shape[i] = obj.shape[i];
                header.data_size *= obj.shape[i];
            }
            header.buffer_size = header.data_size + sizeof(hdc_header_t);
            std::vector<char> buffer(header.buffer_size);
            memcpy(buffer.data(),&header,sizeof(hdc_header_t));
            memcpy(buffer.data()+sizeof(hdc_header_t),obj.data,header.data_size);
            storage->set(uuid, buffer.data(), header.buffer_size);
        }
};

bool HDC::is_terminal() const
{
    auto t = get_type();
    return (t != HDC_STRUCT && t != HDC_LIST);
}

HDC HDC::copy(bool deep_copy)
{
    return copy(*this,deep_copy);
}

HDC HDC::copy(const HDC& h, bool deep_copy) {
    auto storage = h.get_storage();
    auto h_buffer = h.get_buffer();
    hdc_header_t h_header;
    memcpy(&h_header,h_buffer,sizeof(hdc_header_t));

    std::vector<char> c_buffer(h_header.buffer_size);
    hdc_header_t c_header;
    memcpy(&c_header,h_buffer,sizeof(hdc_header_t));

    auto c_uuid = generate_uuid_str();

    if ((h_header.type != HDC_STRUCT && h_header.type != HDC_LIST) || !deep_copy) {
        memcpy(c_buffer.data(),h_buffer,h_header.buffer_size);
    } else {
        memcpy(c_buffer.data(),h_buffer,sizeof(hdc_header_t));
        bip::managed_external_buffer h_segment(bip::open_only, h_buffer + sizeof(hdc_header_t),
                                                 h_header.data_size);
        hdc_map_t* h_children = h_segment.find<hdc_map_t>("d").first;


        bip::managed_external_buffer c_segment(bip::create_only, c_buffer.data() + sizeof(hdc_header_t),
                                             h_header.data_size);
        hdc_map_t* c_children = c_segment.construct<hdc_map_t>("d")(hdc_map_t::ctor_args_list(),
                                                         hdc_map_t::allocator_type(c_segment.get_segment_manager()));
        // The children seem to have to be copied before iterating over them, otherwise the recursion fails and we get multiple instances of deepest terminal nodes spreaded all over the tree copy..
        std::vector<std::pair<std::string,std::string>> children_list;
        for (hdc_map_t::iterator it = h_children->get<by_key>().begin(); it != h_children->get<by_key>().end(); ++it) {
            children_list.push_back({it->key.c_str(),it->address.c_str()});
        }
        for (auto it: children_list) {
            HDC n(storage, it.second);
            HDC cpy = copy(n,deep_copy);
            record rec(it.first, cpy.get_uuid().c_str(), c_segment.get_segment_manager());
            c_children->insert(rec);
        }
    }
    storage->set(c_uuid,c_buffer.data(),h_header.buffer_size);
    return HDC(storage,c_uuid);
}
