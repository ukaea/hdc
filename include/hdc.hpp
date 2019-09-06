#ifndef HDC_HPP
#define HDC_HPP

#include <iostream>

// marray - array views
#include "andres/marray.hxx"

// some other stuff -- to be reduced later
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <json/json.h>
#include <list>
#include <map>
#include <sstream>
#include <cstdbool>
#include <unordered_map>
#include <vector>
// our stuff
#include "hdc_errors.hpp"
#include "hdc_global.hpp"
#include "hdc_helpers.h"
#include "hdc_map.h"
#include "hdc_serializer.h"
#include "hdc_storage.h"
#include "hdc_types.h"
#include "hdc_utils.h"

#ifdef _USE_HDF5
#  include <H5Cpp.h>
#endif

// #define DEBUG

using namespace std;
extern HDCGlobal hdc_global;

using byte = unsigned char;
using hdc_index_t = boost::variant<size_t, std::string>;
using hdc_path_t = std::list<hdc_index_t>;

class HDC {
private:
    std::string uuid;
    HDCStorage* storage;

/* ------------------------------- methods ----------------------------------------- */

    /**
    * @brief Adds node as child under the path
    *
    * @param path p_path: path under which to add node
    * @param n p_n: node
    */
    void add_child(hdc_path_t& path, HDC& n);

    /**
    * @brief Sets child under the path
    *
    * @param path p_path: path uder which to set node
    * @param n p_n: node
    */
    void set_child(hdc_path_t& path, HDC& n);

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    */
    void delete_child(hdc_path_t& path);

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @return HDC
    */
    HDC get(hdc_path_t& path);

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @return HDC
    */
    HDC get(hdc_path_t& path) const;

    /**
    * @brief ...
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return bool
    */
    bool exists_single(hdc_index_t index) const;

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @param n p_n: HDC node
    */
    void add_child_single(const std::string& path, HDC& n);

    /**
    * @brief Returns copy of header containing metadata
    *
    * @return hdc_header_t
    */
    hdc_header_t get_header() const;

    /**
    * @brief Returns pointer to header containing metadata
    *
    * @return hdc_header_t
    */
    hdc_header_t* get_header_ptr() const;

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @param n p_n: HDC node
    */
    void set_child_single(hdc_index_t path, HDC& n);

public:
    /**
    * @brief ...
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return HDC
    */
    HDC get_single(hdc_index_t index);

    /**
    * @brief ...
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return HDC
    */
    HDC get_single(hdc_index_t index) const;

    /**
    * @brief Creates empty HDC with specified buffer size
    *
    * @param byte_size p_byte_size:...
    */
    explicit HDC(size_t byte_size);

    /**
    * @brief Default constructor. Creates empty HDC
    *
    */
    HDC();

    /**
    * @brief Creates empty HDC with specified type and shape
    *
    * @param shape p_shape: Shape of the data
    * @param type p_type: Type of the data (e.g. HDC_INT32)
    * @param flags p_flags: Flags the node should have (e.g. HDCFortranOrder)
    */
    HDC(std::vector<size_t>& shape, hdc_type_t type, hdc_flags_t flags = HDCDefault);

    explicit HDC(hdc_data_t obj);

    /**
    * @brief Constructor from string
    *
    * @param str p_str:...
    */
    HDC(const std::string& str);

    /**
    * @brief Copy contructor
    *
    * @param h p_h: pointer to HDC object
    */
    HDC(const HDC& h);

    /**
    * @brief Deserializing constructor
    *
    * @param _storage p__storage:...
    * @param _uuid p__uuid:...
    */
    HDC(HDCStorage* _storage, const std::string& _uuid);

    /**
    * @brief Constructs HDC object from hdc_obj_t struct - C/FORTRAN interoperability
    *
    * @param obj p_obj:...
    */
    explicit HDC(hdc_t& obj);

    /**
    * @brief Constructs HDC object from scalar and type - MATLAB interoperability
    *
    * @param data p_data:...
    * @param t p_t:...
    */
    HDC(void* data, hdc_type_t t);

    /**
    * @brief Constructs HDC object from std::vector
    *
    * @param T p_T: data type
    * @param data p_data: vector with data
    * @param flags p_flags: flags
    */
    template <typename T>
    explicit HDC(std::vector<T> data, hdc_flags_t flags = HDCDefault)
    {
        HDC_STORAGE_INIT()
        size_t rank = 1;
        size_t data_size = sizeof(T) * data.size();
        auto buffer_size = data_size + sizeof(hdc_header_t);
        std::vector<char> buffer(buffer_size, 0);
        auto header = reinterpret_cast<hdc_header_t*>(buffer.data());
        header->type = to_typeid(data[0]);
        header->flags = flags;
        header->rank = rank;
        header->data_size = data_size;
        header->buffer_size = buffer_size;
        header->shape[0] = data.size();
        memcpy(buffer.data() + sizeof(hdc_header_t), data.data(), data_size);
        uuid = generate_uuid_str();
        storage = hdc_global.storage;
        storage->set(uuid, buffer.data(), buffer_size);
    }

    /**
    * @brief Destructor
    *
    */
    ~HDC();

    /**
    * @brief Parses command line arguments
    *
    * @param argc p_argc:...
    * @param argv p_argv:...
    */
    static void parse_cmdline(int argc, const char* argv[]);

    /**
    * @brief Loads settings from file
    *
    * @param configPath p_configPath:...
    */
    static void load_config(std::string configPath = "./hdc.conf:~/.config/hdc.conf");

    /**
    * @brief Searches for available storage plugins
    *
    * @param searchPath p_searchPath:...
    */
    static std::unordered_map<std::string, std::string> search_plugins(const std::string& customSearchPath = "");

    /**
    * @brief Sets the default storage to be used.
    *
    * @param storage p_storage:...
    */
    static void set_storage(std::string storage = "umap", std::string storage_cmd = "");

    /**
    * @brief Returns a directory path where the HDC library is loaded from.
    *
    * @return std::string
    */
    static std::string get_library_dir();

    /**
    * @brief Initializes hdc_global.storage  -- mainly due to C and Fortran
    *
    * @param storage p_storage:...
    * @param storage_options p_storage_options:...
    */
    static void init(const std::string& storage = "", const std::string& storage_options = "");

    /**
    * @brief Sets the default storage options, builds the boost::property_tree with default settings structure to be redefined later by parse_cmdline(), load_config() or set_storage() functions.
    *
    * @param storage p_storage:...
    * @param storage_options p_storage_options:...
    */
    static void set_default_storage_options(const std::string& storage, const std::string& storage_options = "");

    /**
    * @brief Returns map of children if there are any, otherwise it returns empty map
    *
    * @return std::map< HDC >
    */
    const std::map<std::string, HDC> get_children() const;

    /**
    * @brief Returns map of children if there are any, otherwise it returns empty map
    *
    * @return int
    */
    const std::vector<HDC> get_slices() const;

    /**
    * @brief Makes copy of object. Copies at least node buffer. If deep_copy specified, it also copies all children nodes.
    *
    * @param deep_copy p_deep_copy:...
    * @return HDC
    */
    HDC copy(bool deep_copy = true);

    /**
    * @brief removes node's children
    *
    */
    void clean();

    /**
    * @brief Cleans up the hdc_global.storage  -- mainly due to C and Fortran
    *
    */
    static void destroy();

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @return bool
    */
    bool exists(hdc_path_t& path) const;

    /**
    * @brief ...
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return bool
    */
    bool exists(size_t index) const;

    /**
    * @brief ...
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return HDC
    */
    HDC get(size_t index);

    /**
    * @brief ...
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return HDC
    */
    HDC get(size_t index) const;

    /**
    * @brief Returns the available space in buffer (in bytes)
    *
    * @return size_t
    */
    size_t get_datasize() const;

    /**
    * @brief Returns the the size of object buffer (= header+data, in bytes)
    *
    * @return size_t
    */
    size_t get_size() const;


    /**
    * @brief Returns type of current node.
    *
    * @return size_t
    */
    size_t get_type() const;

    /**
    * @brief Returns the size of a single item in bytes
    *
    * @return size_t
    */
    size_t get_itemsize() const;

    /**
    * @brief Returns object flags (i.e. array ordering)
    *
    * @return size_t
    */
    size_t get_flags() const;

    /**
    * @brief Returns true if type is not one of HDC_LIST or HDC_STRUCT
    *
    * @return bool
    */
    bool is_terminal() const;

    /**
    * @brief ...
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return HDC
    */
    HDC get_or_create(size_t index);

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @return HDC
    */
    HDC get_or_create(const std::string& path);

    /**
     * @brief ...
     *
     * @param path p_path: path in the subtree of this node
     * @return HDC
     */
    HDC get_or_create(hdc_path_t& path);

    /**
    * @brief Stores data in node's buffer
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return HDC
    */
    HDC operator[](size_t index) const;

    /**
    * @brief ...
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return HDC&
    */
    HDC operator[](size_t index);

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @return HDC
    */
    HDC operator[](const std::string& path) const;

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @return HDC&
    */
    HDC operator[](const std::string& path);

    /**
    * @brief ...
    *
    * @param T p_T: Desired data type.
    * @param d p_d:...
    * @return HDC&
    */
    template <typename T>
    HDC& operator=(T d)
    {
        set_data(d);
        return *this;
    }

    /**
    * @brief ...
    *
    * @param str p_str:...
    * @return HDC&
    */
    HDC& operator=(char const* str);
    /**
    * @brief ...
    *
    * @param str p_str:...
    * @return HDC&
    */
    HDC& operator=(const std::string& str);
    /**
    * @brief ...
    *
    * @param other p_other:...
    * @return HDC&
    */
    HDC& operator=(const HDC& other);

    /**
    * @brief ...
    *
    * @param T p_T: Desired data type.
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param flags p_flags: Flags the node should have (e.g. HDCFortranOrder)
    */
    template <typename T>
    void set_data(std::vector<size_t>& shape, T* data, hdc_flags_t flags = HDCDefault)
    {
        auto rank = shape.size();
        auto buffer = get_buffer();
        auto header = reinterpret_cast<hdc_header_t*>(buffer);
        // Start with determining of the buffer size
        auto data_size = sizeof(T);
        for (size_t i = 0; i < rank; i++) data_size *= shape[i];
        auto buffer_size = data_size + sizeof(hdc_header_t);
        if (header->buffer_size == buffer_size) {
            storage->lock(uuid);
            memcpy(buffer + sizeof(hdc_header_t), data, data_size);
            storage->unlock(uuid);
            return;
        } else {
            std::vector<char> new_buffer(buffer_size);
            header = reinterpret_cast<hdc_header_t*>(new_buffer.data());
            if (header->flags & HDCExternal) {
                throw HDCException("set_data(): I can't enlarge your external buffer for you...");
            }
            header->buffer_size = buffer_size;
            header->data_size = data_size;
            memset(header->shape, 0, HDC_MAX_DIMS * sizeof(size_t));
            for (size_t i = 0; i < rank; i++) header->shape[i] = shape[i];
            header->flags = flags;
            header->type = to_typeid(data[0]);
            header->rank = rank;
            memcpy(new_buffer.data() + sizeof(hdc_header_t), data, data_size);
            storage->set(uuid, new_buffer.data(), buffer_size);
            return;
        }
    }

    /**
    * @brief ...
    *
    * @param T p_T: Desired data type.
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param flags p_flags: Flags the node should have (e.g. HDCFortranOrder)
    */
    template <typename T>
    void set_external(std::vector<size_t>& shape, T* data, hdc_flags_t flags = HDCDefault)
    {
        auto rank = shape.size();
        auto buffer = get_buffer();
        auto header = reinterpret_cast<hdc_header_t*>(buffer);
        // Start with determining of the buffer size
        auto data_size = sizeof(void*);
        auto buffer_size = data_size + sizeof(hdc_header_t);
        if (header->buffer_size == buffer_size) {
            storage->lock(uuid);
            memcpy(buffer + sizeof(hdc_header_t), &data, data_size);
            storage->unlock(uuid);
            return;
        } else {
            std::vector<char> new_buffer(buffer_size);
            header = reinterpret_cast<hdc_header_t*>(new_buffer.data());
            header->buffer_size = buffer_size;
            header->data_size = data_size;
            memset(header->shape, 0, HDC_MAX_DIMS * sizeof(size_t));
            for (size_t i = 0; i < rank; i++) header->shape[i] = shape[i];
            header->flags = flags | HDCExternal;
            header->type = to_typeid(data[0]);
            header->rank = rank;
            memcpy(new_buffer.data() + sizeof(hdc_header_t), &data, data_size);
            storage->set(uuid, new_buffer.data(), buffer_size);
            return;
        }
    }

    /**
    * @brief ...
    *
    * @param T p_T: Desired data type.
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param flags p_flags: Flags the node should have (e.g. HDCFortranOrder)
    */
    template <typename T>
    void set_data(initializer_list<size_t> shape, T* data, hdc_flags_t flags = HDCDefault)
    {
        std::vector<size_t> _shape = shape;
        set_data(_shape, data, flags);
    }

    /**
     * @brief ...
     *
    * @param T p_T: Desired data type.
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param flags p_flags: Flags the node should have (e.g. HDCFortranOrder)
     */
    template <typename T>
    void set_external(initializer_list<size_t> shape, T* data, hdc_flags_t flags = HDCDefault)
    {
        std::vector<size_t> _shape = shape;
        set_external(_shape, data, flags);
    }

    /**
    * @brief ...
    *
    * @param T p_T: Desired data type.
    * @param data p_data: Pointer to data
    * @param flags p_flags: Flags the node should have (e.g. HDCFortranOrder)
    */
    template <typename T>
    void set_data(initializer_list<T> data, hdc_flags_t flags = HDCDefault)
    {
        DEBUG_STDOUT(std::string(
                             "template<typename T> void set_data(initializer_list<T> data, hdc_flags_t flags = HDCDefault)") +
                             to_string(data[0]));
        vector<T> vec = data;
        set_data(1, { vec.size() }, &vec[0], flags);
    }

    /**
    * @brief Sets data to current node from vector<T> data. This function is primarily designed for interoperability with Python
    *
    * @param T p_T: Desired data type.
    * @param data p_data: Pointer to data
    */
    template <typename T>
    void set_data(vector<T> data)
    {
        DEBUG_STDOUT(std::string("template <typename T> void set_data(vector<T> data)") + to_string(data[0]));
        if (get_children_ptr() != nullptr) {
            std::cout << "The node has already children set..." << std::endl;
            return;
        }
        std::vector<size_t> shape = { data.size() };
        set_data<T>(shape, &data[0]);
        return;
    }

    /**
    * @brief Sets string to current node
    *
    * @param str p_str: string to set.
    */
    void set_string(const std::string& str)
    {
        if (storage->has(uuid)) {
            storage->remove(uuid);
        }
        auto data_size = str.length() + 1;
        auto buffer_size = data_size + sizeof(hdc_header_t);
        std::vector<char> buffer(buffer_size);
        auto header = reinterpret_cast<hdc_header_t*>(buffer.data());
        header->data_size = data_size;
        header->type = HDC_STRING;
        header->rank = 1;
        header->shape[0] = str.length();
        header->buffer_size = buffer_size;
        memcpy(buffer.data() + sizeof(hdc_header_t), str.c_str(), header->data_size);
        storage->set(uuid, buffer.data(), header->buffer_size);
    }

    /**
    * @brief ...
    *
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param type p_type: Type of the data (e.g. HDC_INT32)
    * @param flags p_flags: Flags the node should have (e.g. HDCFortranOrder)
    */
    void set_data_c(std::vector<size_t>& shape, void* data, hdc_type_t type, hdc_flags_t flags = HDCDefault);
    /**
    * @brief ...
    *
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param type p_type: Type of the data (e.g. HDC_INT32)
    * @param flags p_flags: Flags the node should have (e.g. HDCFortranOrder)
    */
    void set_external_c(std::vector<size_t>& shape, void* data, hdc_type_t type, hdc_flags_t flags = HDCDefault);
    /**
     * @brief ...
     *
     * @param shape p_shape:...
     * @param data p_data:...
     * @param kind p_kind:...
     * @param itemsize p_itemsize:...
     * @param flags p_flags:...
     */
    void
    set_data_Py(std::vector<size_t>& shape, void* data, char kind, int8_t itemsize, hdc_flags_t flags = HDCDefault);
    /**
     * @brief ...
     *
     * @param shape p_shape:...
     * @param data p_data:...
     * @param kind p_kind:...
     * @param itemsize p_itemsize:...
     * @param flags p_flags:...
     */
    void
    set_external_Py(std::vector<size_t>& shape, void* data, char kind, int8_t itemsize, hdc_flags_t flags = HDCDefault);
    /**
    * @brief ...
    *
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param type p_type: Type of the data (e.g. HDC_INT32)
    * @param flags p_flags: Flags the node should have (e.g. HDCFortranOrder)
    */
    void set_data_c(std::vector<size_t>& shape, const void* data, hdc_type_t type, hdc_flags_t flags = HDCDefault);

    /**
    * @brief Sets scalar data to given node.
    *
    * @param T p_T: Desired data type.
    * @param data p_data: Pointer to data
    */
    template <typename T>
    void set_data(T data)
    {
        auto data_size = sizeof(T);
        auto buffer_size = data_size + sizeof(hdc_header_t);
        std::vector<char> buffer(buffer_size);
        auto header = reinterpret_cast<hdc_header_t*>(buffer.data());
        header->type = to_typeid(data);
        header->data_size = data_size;
        header->buffer_size = buffer_size;
        memcpy(buffer.data() + sizeof(hdc_header_t), &data, data_size);
        storage->set(uuid, buffer.data(), buffer_size);
    }

    /**
    * @brief Sets scalar data to given node - UDA version.
    *
    * @param data p_data: Pointer to data
    * @param _type p__type:...
    */
    void set_data(const unsigned char* data, hdc_type_t _type)
    {
        auto data_size = hdc_sizeof(_type);
        auto buffer_size = data_size + sizeof(hdc_header_t);
        std::vector<char> buffer(buffer_size);
        auto header = reinterpret_cast<hdc_header_t*>(buffer.data());
        header->type = _type;
        header->data_size = data_size;
        header->buffer_size = buffer_size;
        memcpy(buffer.data() + sizeof(hdc_header_t), data, data_size);
        storage->set(uuid, buffer.data(), buffer_size);
    }

    /**
    * @brief Sets scalar data to given node - UDA version.
    *
    * @param data p_data: Pointer to data
    * @param type p_type: Type of the data (e.g. HDC_INT32)
    */
    void set_data(void* data, hdc_type_t type)
    {
        return this->set_data((const unsigned char*)data, type);
    }

    /**
    * @brief Returns shape of current node.
    *
    * @return std::vector< size_t >
    */
    std::vector<size_t> get_shape() const;

    /**
    * @brief ...
    *
    * @return std::vector< size_t >
    */
    std::vector<size_t> get_strides() const;
    /**
    * @brief ...
    *
    * @return bool
    */
    bool is_external() const;

    /**
    * @brief ...
    *
    * @return bool
    */
    bool is_readonly() const;

    /**
    * @brief ...
    *
    * @return bool
    */
    bool is_fortranorder() const;

    /**
    * @brief ...
    *
    */
    void print_info() const;

    /**
    * @brief Adds HDC subtree as child with given path. If neccessary, recursively creates subnodes. Reference version.
    *
    * @param path p_path: path in the subtree of this node
    * @param n p_n: HDC node
    */
    void add_child(const std::string& path, HDC& n);

    /**
    * @brief Sets HDC subtree to given path.
    *
    * @param path p_path: path in the subtree of this node
    * @param n p_n: HDC node
    */
    void set_child(const std::string& path, HDC& n);

    /**
    * @brief ...
    *
    * @return hdc_obj_t
    */
    hdc_t as_obj();

    /**
    * @brief ...
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @param n p_n: HDC node
    */
    void set_child(size_t index, HDC& n);

    /**
    * @brief Deletes HDC subtree.
    *
    * @param path p_path: path in the subtree of this node
    */
    void delete_child(const std::string& path);

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @return HDC
    */
    HDC get(const std::string& path);

    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @return HDC
    */
    HDC get(const std::string& path) const;

    /**
    * @brief Returns true if subtree with given path with exists and false otherwise.
    *
    * @param path p_path: path in the subtree of this node
    * @return bool
    */
    bool exists(const std::string& path) const;

    /**
    * @brief Inserts node to i-th slice of current node.
    *
    * @param i p_i:...
    * @param h p_h:...
    */
    void insert(size_t i, HDC& h);

    /**
    * @brief Appends given node as next available slice (similar to push_back() method seen in C++ STL containers).
    *
    * @param h p_h:...
    */
    void append(HDC& h);

    /**
    * @brief Sets HDC type of current node.
    *
    * @param type p_type: Type of the data (e.g. HDC_INT32)
    */
    void set_type(hdc_type_t type);

    /**
    * @brief Returns true if node is empty.
    *
    * @return bool
    */
    bool is_empty() const;

    /**
    * @brief Returns number of dimensions of node under path.
    *
    * @return size_t
    */
    size_t get_rank() const;

    /**
    * @brief Returns desirely typed pointer to data of this node.
    *
    * @param T p_T: Desired data type.
    * @return T
    */
    template <typename T>
    T* as() const
    {
        auto buffer = get_buffer();
        auto header = reinterpret_cast<hdc_header_t*>(buffer);
        auto data = buffer + sizeof(hdc_header_t);
        if (header->type == HDC_STRUCT || header->type == HDC_LIST) {
            throw HDCException("This is not a terminal node...");
        }
        DEBUG_STDOUT(std::string("as<") + get_type_str() + ">()");
        if (!storage->has(uuid)) {
            throw HDCException("as(): Not found: " + uuid + "\n");
        }
        T tp{};
        if (header->type != to_typeid(tp)) {
            std::cerr << "*** " << header->type << " " << to_typeid(tp) << std::endl;
            throw HDCException("as() stored and requested types do not match\n");
        }
        if (header->flags & HDCExternal) {
            T* result;
            memcpy(&result, data, sizeof(result));
            return result;
        } else {
            return reinterpret_cast<T*>(data);
        }
    }

    /**
     * @brief Return void pointer to data of this node.
     *
     * @return void*
     */
    void* as_void_ptr() const
    {
        auto buffer = get_buffer();
        auto data = buffer + sizeof(hdc_header_t);
        hdc_header_t header = get_header(); // This is needed in Python for some reason
        if (header.type == HDC_STRUCT || header.type == HDC_LIST) {
            throw std::runtime_error("This is not a terminal node...");
        }
        DEBUG_STDOUT(std::string("as<") + get_type_str() + ">()");
        if (!storage->has(uuid)) {
            throw HDCException("as_void_ptr(): Not found: " + uuid + "\n");
        }
        if (header.flags & HDCExternal) {
            void* result;
            memcpy(&result, data, sizeof(result));
            return result;
        } else {
            return reinterpret_cast<void*>(data);
        }
    }

    /**
     * @brief Returns vector with data of this node casted to a given type. Unlike as(), the data are always copied.
     *
     * @param T p_T: Desired data type.
     * @return std::vector< T >
     */
    template <typename T>
    std::vector<T> as_vector() const
    {
        auto buffer = get_buffer();
        auto header = reinterpret_cast<hdc_header_t*>(buffer);
        if (header->type == HDC_STRUCT || header->type == HDC_LIST) {
            throw std::runtime_error("This is not a terminal node...");
        }
        DEBUG_STDOUT(std::string("as_vector<") + get_type_str() + ">()");
        if (!storage->has(uuid)) {
            throw HDCException("as_vector(): Not found: " + uuid + "\n");
        }
        auto data = buffer + sizeof(hdc_header_t);
        auto size_elem = hdc_sizeof(header->type);
        size_t n_elem = 0;
        if (size_elem) n_elem = header->data_size / size_elem;
        std::vector<T> result{ n_elem };
        if (header->type == HDC_INT8) {
            // return std::vector<T>(reinterpret_cast<int8_t*>(data),reinterpret_cast<int8_t*>(data)+n_elem); // does not work on bionic
            for (size_t i = 0; i < n_elem; i++) result[i] = reinterpret_cast<int8_t*>(data)[i];
        } else if (header->type == HDC_INT16) {
            for (size_t i = 0; i < n_elem; i++) result[i] = reinterpret_cast<int16_t*>(data)[i];
        } else if (header->type == HDC_INT32) {
            for (size_t i = 0; i < n_elem; i++) result[i] = reinterpret_cast<int32_t*>(data)[i];
        } else if (header->type == HDC_INT64) {
            for (size_t i = 0; i < n_elem; i++) result[i] = reinterpret_cast<int64_t*>(data)[i];
        } else if (header->type == HDC_UINT8) {
            for (size_t i = 0; i < n_elem; i++) result[i] = reinterpret_cast<uint8_t*>(data)[i];
        } else if (header->type == HDC_UINT16) {
            for (size_t i = 0; i < n_elem; i++) result[i] = reinterpret_cast<uint16_t*>(data)[i];
        } else if (header->type == HDC_UINT32) {
            for (size_t i = 0; i < n_elem; i++) result[i] = reinterpret_cast<uint32_t*>(data)[i];
        } else if (header->type == HDC_UINT64) {
            for (size_t i = 0; i < n_elem; i++) result[i] = reinterpret_cast<uint64_t*>(data)[i];
        } else if (header->type == HDC_FLOAT) {
            for (size_t i = 0; i < n_elem; i++) result[i] = reinterpret_cast<float*>(data)[i];
        } else if (header->type == HDC_DOUBLE) {
            for (size_t i = 0; i < n_elem; i++) result[i] = reinterpret_cast<double*>(data)[i];
        } else {
            throw HDCException(
                    "as_vector(): requested unknown data conversion. This works for numerical types only...\n");
        }
        return result;
    }

    /**
    * @brief Returns scalar value.
    *
    * @param T p_T: Desired data type.
    * @return T
    */
    template <typename T>
    T as_scalar() const
    {
        auto buffer = get_buffer();
        auto header = reinterpret_cast<hdc_header_t*>(buffer);
        if (header->type == HDC_STRUCT || header->type == HDC_LIST) {
            throw std::runtime_error("This is not a terminal node...");
        }
        DEBUG_STDOUT(std::string("as<") + get_type_str() + ">()");
        if (!storage->has(uuid)) {
            throw HDCException("as_scalar(): Not found: " + uuid + "\n");
        }
        T result;
        memcpy(&result, buffer + sizeof(hdc_header_t), sizeof(T));
        return result;
        //return *reinterpret_cast<T>(get_buffer()+sizeof(hdc_header_t));
    }

    /**
    * @brief Returns string. Needs to have separate function
    *
    * @return const std::__cxx11::string
    */
    std::string as_string() const;

    /**
    * @brief Dumps JSON to std::cout
    *
    */
    void dump() const;

    /**
    * @brief Returns string representing data/node type.
    *
    * @return const char*
    */
    const char* get_type_str() const;
    /**
    * @brief Returns vector of keys of a struct node and empty vector otherwise.
    *
    * @return std::vector< std::__cxx11::string >
    */
    std::vector<std::string> keys() const;

    /**
    * @brief ...
    *
    * @return char*
    */
    char* get_buffer() const;

    /**
    * @brief ...
    *
    * @return std::__cxx11::string
    */
    std::string get_uuid() const;

    /**
    * @brief ...
    *
    * @param extra_size p_extra_size:...
    */
    void grow(size_t extra_size);

    /**
    * @brief "allocator stuff"
    *
    * @return boost::interprocess::managed_external_buffer
    */
    bip::managed_external_buffer get_segment() const;

    /**
    * @brief ...
    *
    * @return hdc_map_t*
    */
    hdc_map_t* get_children_ptr() const;

    /**
    * @brief ...
    *
    * @param children p_children:...
    * @return std::__cxx11::string
    */
    static string hdc_map_to_json(hdc_map_t& children);

    /**
    * @brief ...
    *
    * @param old_buffer p_old_buffer:...
    * @param extra_size p_extra_size:...
    * @return char*
    */
    static std::vector<char> buffer_grow(char* old_buffer, size_t extra_size);

    /**
    * @brief ...
    *
    * @param str p_str:...
    * @param datapath p_datapath:...
    * @return HDC
    */
    static HDC load(const std::string& uri, const std::string& datapath = "");

    void save(const std::string& uri) const;

    static HDC deserialize(const std::string& protocol, const std::string& string);

    std::string serialize(const std::string& protocol) const;

    /**
    * @brief Returns copy of HDC object, if deep_copy == true provides deep copy (i.e. copy of current node and also all children)
    *
    * @param h p_h:...
    * @param deep_copy p_deep_copy:...
    * @return HDC
    */
    static HDC copy(const HDC& h, bool deep_copy = false);

    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data: Void pointer to data
    * @return HDC
    */
    static HDC make_scalar(void* data, hdc_type_t t);

    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data:...
    * @return HDC
    */
    static HDC make_scalar(float data);

    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data:...
    * @return HDC
    */
    static HDC make_scalar(double data);
    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data:...
    * @return HDC
    */
    static HDC make_scalar(bool data);

    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data:...
    * @return HDC
    */
    static HDC make_scalar(int8_t data);

    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data:...
    * @return HDC
    */
    static HDC make_scalar(int16_t data);

    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data:...
    * @return HDC
    */
    static HDC make_scalar(int32_t data);

    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data:...
    * @return HDC
    */
    static HDC make_scalar(int64_t data);

    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data:...
    * @return HDC
    */
    static HDC make_scalar(uint8_t data);

    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data:...
    * @return HDC
    */
    static HDC make_scalar(uint16_t data);

    /**
    * @brief Creates scalar HDC object
    *
    * @param data p_data:...
    * @return HDC
    */
    static HDC make_scalar(uint32_t data);

    /**
    * @brief Creates scalar HDC object
    *
    * @return hdc_data_t
    */
    static HDC make_scalar(uint64_t data);
    /**
     * @brief ...
     *
     * @param obj p_obj:...
     * @return HDC
     */
    static HDC make_external(hdc_data_t obj);

    /**
     * @brief ...
     *
     * @param shape p_shape:...
     * @param type p_type:...
     * @param flags p_flags:...
     * @return HDC
     */
    static HDC make_external(std::vector<size_t>& shape, hdc_type_t type, long flags);

    /**
    * @brief Returns pointer to storage of HDC object
    *
    * @return HDCStorage*
    */
    HDCStorage* get_storage() const { return this->storage; }

    /**
    * @brief Returns storage id
    *
    * @return size_t
    */
    size_t get_storage_id() const { return this->storage->id(); }

    /**
    * @brief ...
    *
    * @return hdc_data_t
    */
    hdc_data_t get_data() const;

    /**
    * @brief ...
    *
    * @param obj p_obj:...
    */
    void set_data(hdc_data_t obj);

    /**
     * @brief ...
     *
     * @param obj p_obj:...
     */
    void set_external(hdc_data_t obj);
};

#endif // HDC_HPP
