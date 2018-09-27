#ifndef HDC_HPP
#define HDC_HPP

#include <iostream>

// marray - array views
#include "andres/marray.hxx"

// Boost
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

// some other stuff -- to be reduced later
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <json/json.h>
#include <exception>

// our stuff
#include "hdc_types.h"
#include "hdc_map.h"
#include "hdc_utils.h"
#include "hdc_storage.h"
#include "hdc_errors.hpp"
#include "hdc_helpers.h"

#ifdef _USE_HDF5
#include <H5Cpp.h>
#endif

// #define DEBUG


using namespace std;
namespace pt = boost::property_tree;

//this will hold all the settings, instead of boost::property_tree json we will use the jsoncpp for actual reading of them.
extern pt::ptree* options;
//this is default global storage
extern HDCStorage* global_storage;
//list of found plugins
extern std::unordered_map<std::string,std::string> avail_stores;

using byte = unsigned char;
using hdc_index_t = boost::variant<size_t,std::string>;
using hdc_path_t = std::list<hdc_index_t>;

class HDC
{
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
    void add_child(hdc_path_t& path, HDC* n);
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
    * @brief Sets child under the path
    *
    * @param path p_path: path uder which to set node
    * @param n p_n: node
    */
    void set_child(hdc_path_t& path, HDC* n);
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
    * @return HDC*
    */
    HDC* get_ptr(hdc_path_t& path);
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
    * @return const HDC
    */
    const HDC get(hdc_path_t& path) const;
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
    * @brief ...
    *
    * @return hdc_header_t
    */
    hdc_header_t get_header() const;
    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @param n p_n: HDC node
    */
    void set_child_single(hdc_index_t path, HDC& n);
    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @param n p_n: HDC node
    */
    void set_child_single(hdc_index_t path, HDC* n);
    /**
    * @brief ...
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return HDC*
    */
    HDC* get_single_ptr(hdc_index_t index);
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
    * @return const HDC
    */
    const HDC get_single(hdc_index_t index) const;
public:
    /**
    * @brief Creates empty HDC with specified buffer size
    *
    * @param byte_size p_byte_size:...
    */
    HDC(size_t byte_size);
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
    * @param flags p_flags: Fags the node should have (e.g. HDCFortranOrder)
    */
    HDC(std::vector<size_t>& shape, hdc_type_t type,long flags = HDCDefault);
    /**
    * @brief Constructor from string
    *
    * @param str p_str:...
    */
    HDC(const std::string str);
    /**
    * @brief Copy contructor
    *
    * @param h p_h:...
    */
    HDC(HDC* h);
    /**
    * @brief Deserializing constructor
    *
    * @param _storage p__storage:...
    * @param _uuid p__uuid:...
    */
    HDC(HDCStorage* _storage, const std::string& _uuid);

    /**
    * @brief Constructs object from hdc_obj_t struct - C/FORTRAN interoperability
    *
    * @param obj p_obj:...
    */
    HDC(hdc_t& obj);
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
    static void parse_cmdline(int argc, const char *argv[]);
    /**
    * @brief Loads settings from file
    *
    * @param configPath p_configPath:...
    */
    static void load_config(std::string configPath="./hdc.conf:~/.config/hdc.conf");
    /**
    * @brief Searches for available storage plugins
    *
    * @param searchPath p_searchPath:...
    */
    static void search_plugins(string searchPath="./:./plugins:./hdc_plugins:.local/hdc/plugins");
    /**
    * @brief Prints out all found storage plugins.
    *
    */
    static void list_plugins();
    /**
    * @brief Sets the default storage to be used.
    *
    * @param storage p_storage:...
    */
    static void set_storage(std::string storage="umap");
    /**
    * @brief Returns list of all available storage plugins
    *
    * @return std::vector< std::__cxx11::string >
    */
    static std::vector<std::string> get_available_plugins();
    /**
    * @brief Returns a directory path where the HDC library is loaded from.
    *
    * @return std::__cxx11::string
    */
    static std::string get_library_dir(void);
    /**
    * @brief Initializes global_storage  -- mainly due to C and Fortran
    *
    * @param storage p_storage:...
    * @param storage_options p_storage_options:...
    */
    static void init(std::string storage="umap",std::string storage_options="");
    /**
    * @brief Sets the default storage options, builds the boost::property_tree with default settings structure to be redefined later by parse_cmdline(), load_config() or set_storage() functions.
    *
    * @param storage p_storage:...
    * @param storage_options p_storage_options:...
    */
    static void set_default_storage_options(std::string storage="umap", std::string storage_options="");
    void clean();
    /**
    * @brief Cleans up the global_storage  -- mainly due to C and Fortran
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
    * @return HDC*
    */
    HDC* get_ptr(size_t index);
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
    * @return const HDC
    */
    const HDC get(size_t index) const;
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
    HDC get_or_create(hdc_path_t& path);
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
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return HDC*
    */
    HDC* get_or_create_ptr(size_t index);
    /**
    * @brief ...
    *
    * @param path p_path: path in the subtree of this node
    * @return HDC*
    */
    HDC* get_or_create_ptr(const std::string& path);
    /**
    * @brief Returns the data, the pointer is just casted => there is no conversion for now.
    *
    * @param T p_T: Desired data type.
    * @return T*
    */
    template<typename T> T* get_data() const;
    /**
    * @brief Stores data in node's buffer
    *
    * @param index p_index: size_t index if this node has/should have type HDC_LIST, or std::string if this node has/should have type HDC_STRUCT
    * @return const HDC
    */
    const HDC operator[](size_t index) const;
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
    * @return const HDC
    */
    const HDC operator[](const std::string& path) const;
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
    template <typename T> HDC& operator=(T d)
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
    * @param flags p_flags: Fags the node should have (e.g. HDCFortranOrder)
    */
    template<typename T> void set_data(std::vector<size_t>& shape, T* data, hdc_flags_t flags = HDCDefault) {
        auto rank = shape.size();
        std::cerr << "RANK: " << rank << " " << shape[0] << std::endl;

        hdc_header_t header = get_header();
        D(printf("set_data(%d, {%d,%d,%d}, %f)\n",rank,shape[0],shape[1],shape[2],((double*)data)[0]);)
        auto buffer = storage->get(uuid);
        memcpy(&header,buffer,sizeof(hdc_header_t));
        // Start with determining of the buffer size
        size_t data_size = sizeof(T);
        for (size_t i=0;i<rank;i++) data_size *= shape[i];
        size_t buffer_size = data_size + sizeof(hdc_header_t);
        if (header.buffer_size == buffer_size) {
            storage->lock(uuid);
            memcpy(buffer+sizeof(hdc_header_t),data,data_size);
            storage->unlock(uuid);
            return;
        } else {
            header.buffer_size = buffer_size;
            header.data_size = data_size;
            memset(header.shape,0,HDC_MAX_DIMS*sizeof(size_t));
            for (size_t i=0;i<rank;i++) header.shape[i] = shape[i];
            header.flags = flags;
            header.type = to_typeid(data[0]);
            header.rank = rank;
            std::vector<char> buffer(header.buffer_size);
            memcpy(buffer.data(),&header,sizeof(hdc_header_t));
            memcpy(buffer.data()+sizeof(hdc_header_t),data,header.data_size);
            storage->set(uuid,buffer.data(),header.buffer_size);
            return;
        }
    }

    /**
    * @brief ...
    *
    * @param T size_t: Desired rank.
    * @param T p_T: Desired data type.
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param flags p_flags: Fags the node should have (e.g. HDCFortranOrder)
    */
    template<typename T> void set_data(size_t rank, std::vector<size_t>& shape, T* data, hdc_flags_t flags = HDCDefault) {
        hdc_header_t header = get_header();
        D(printf("set_data(%d, {%d,%d,%d}, %f)\n",rank,shape[0],shape[1],shape[2],((double*)data)[0]);)
        auto buffer = storage->get(uuid);
        memcpy(&header,buffer,sizeof(hdc_header_t));
        // Start with determining of the buffer size
        size_t data_size = sizeof(T);
        for (size_t i=0;i<rank;i++) data_size *= shape[i];
        size_t buffer_size = data_size + sizeof(hdc_header_t);
        if (header.buffer_size == buffer_size) {
            storage->lock(uuid);
            memcpy(buffer+sizeof(hdc_header_t),data,data_size);
            storage->unlock(uuid);
            return;
        } else {
            header.buffer_size = buffer_size;
            header.data_size = data_size;
            memset(header.shape,0,HDC_MAX_DIMS*sizeof(size_t));
            for (size_t i=0;i<rank;i++) header.shape[i] = shape[i];
            header.flags = flags;
            header.type = to_typeid(data[0]);
            header.rank = rank;
            std::vector<char> buffer(header.buffer_size);
            memcpy(buffer.data(),&header,sizeof(hdc_header_t));
            memcpy(buffer.data()+sizeof(hdc_header_t),data,header.data_size);
            storage->set(uuid,buffer.data(),header.buffer_size);
            return;
        }
    }
    /**
    * @brief ...
    *
    * @param T p_T: Desired data type.
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param flags p_flags: Fags the node should have (e.g. HDCFortranOrder)
    */
    template<typename T> void set_data(initializer_list<size_t> shape, T* data, hdc_flags_t flags = HDCDefault) {
        set_data(shape,data,flags);
    };

    /**
    * @brief ...
    *
    * @param T p_T: Desired data type.
    * @param data p_data: Pointer to data
    * @param flags p_flags: Fags the node should have (e.g. HDCFortranOrder)
    */
    template<typename T> void set_data(initializer_list<T> data, hdc_flags_t flags = HDCDefault) {
        DEBUG_STDOUT("template<typename T> void set_data(initializer_list<T> data, hdc_flags_t flags = HDCDefault)"+to_string(data[0]));
        vector<T> vec = data;
        set_data(1,{vec.size()},&vec[0],flags);
    };

    /**
    * @brief Sets data to current node from vector<T> data. This function is primarily designed for interoperability with Python
    *
    * @param T p_T: Desired data type.
    * @param data p_data: Pointer to data
    */
    template <typename T> void set_data(vector<T> data)
    {
        DEBUG_STDOUT("template <typename T> void set_data(vector<T> data)"+to_string(data[0]));
        if (get_children_ptr() != nullptr) {
            std::cout << "The node has already children set..." << std::endl;
            return;
        }
        size_t shape[1] = {data.size()};
        set_data<T>(1,shape,&data[0]);
        return;
    };

    /**
    * @brief Sets string to current node
    *
    * @param str p_str: string to ne set.
    */
    void set_string(const std::string& str) {
        hdc_header_t header = get_header();
        if (storage->has(uuid)) {
            storage->remove(uuid);
        }
        memset(&header,0,sizeof(hdc_header_t));
        header.data_size = str.length()+1;
        header.type = HDC_STRING;
        header.rank = 1;
        header.shape[0] = str.length();
        header.buffer_size = header.data_size + sizeof(hdc_header_t);
        std::vector<char> buffer(header.buffer_size);
        memcpy(buffer.data(),&header,sizeof(hdc_header_t));
        memcpy(buffer.data()+sizeof(hdc_header_t),str.c_str(),header.data_size);
        storage->set(uuid,buffer.data(),header.buffer_size);
    };
    /**
    * @brief ...
    *
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param type p_type: Type of the data (e.g. HDC_INT32)
    * @param flags p_flags: Fags the node should have (e.g. HDCFortranOrder)
    */
    void set_data_c(std::vector<size_t>& shape, void* data, hdc_type_t type, hdc_flags_t flags = HDCDefault);
    /**
    * @brief ...
    *
    * @param shape p_shape: Shape of the data
    * @param data p_data: Pointer to data
    * @param type p_type: Type of the data (e.g. HDC_INT32)
    * @param flags p_flags: Fags the node should have (e.g. HDCFortranOrder)
    */
    void set_data_c(std::vector<size_t>& shape, const void* data, hdc_type_t type, hdc_flags_t flags = HDCDefault);
    /** Sets scalar data to given node. */
    template <typename T>
    /**
    * @brief ...
    *
    * @param T p_T: Desired data type.
    * @param data p_data: Pointer to data
    */
    void set_data(T data) {
        hdc_header_t header = get_header();
        memset(&header,0,sizeof(hdc_header_t));
        header.type = to_typeid(data);
        header.data_size = sizeof(T);
        header.buffer_size = header.data_size + sizeof(hdc_header_t);
        std::vector<char> buffer(header.buffer_size);
        memcpy(buffer.data(),&header,sizeof(hdc_header_t));
        memcpy(buffer.data()+sizeof(hdc_header_t),&data,header.data_size);
        storage->set(uuid,buffer.data(),header.buffer_size);
    }
    /**
    * @brief Sets scalar data to given node - UDA version.
    *
    * @param data p_data: Pointer to data
    * @param _type p__type:...
    */
    void set_data(const unsigned char* data, hdc_type_t _type) {
        hdc_header_t header = get_header();
        memset(&header,0,sizeof(hdc_header_t));
        header.type = _type;
        header.data_size = hdc_sizeof(_type);
        header.buffer_size = header.data_size + sizeof(hdc_header_t);
        std::vector<char> buffer(header.buffer_size);
        memcpy(buffer.data(),&header,sizeof(hdc_header_t));
        memcpy(buffer.data()+sizeof(hdc_header_t),data,header.data_size);
        storage->set(uuid,buffer.data(),header.buffer_size);
    }
    /**
    * @brief Sets scalar data to given node - UDA version.
    *
    * @param data p_data: Pointer to data
    * @param type p_type: Type of the data (e.g. HDC_INT32)
    */
    void set_data(void* data, hdc_type_t type) {
        return this->set_data((const unsigned char*)data,type);
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
    * @brief Adds HDC subtree as child with given path. If neccessary, recursively creates subnodes. Pointer version.
    *
    * @param path p_path: path in the subtree of this node
    * @param n p_n: HDC node
    */
    void add_child(const std::string& path, HDC* n);
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
    * @brief Sets HDC subtree to given path.
    *
    * @param path p_path: path in the subtree of this node
    * @param n p_n: HDC node
    */
    void set_child(const std::string& path, HDC* n);
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
    void set_child(size_t index, HDC* n);
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
    * @brief Returns subtree by path.
    *
    * @param path p_path: path in the subtree of this node
    * @return HDC*
    */
    HDC* get_ptr(const std::string& path);
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
    * @return const HDC
    */
    const HDC get(const std::string& path) const;
    /**
    * @brief Returns true if subtree with given path with exists and false otherwise.
    *
    * @param path p_path: path in the subtree of this node
    * @return bool
    */
    bool exists(const std::string& path) const;
    /** Sets HDC_LIST from std::deque<HDC*> data.*/
    /**
    * @brief ...
    *
    * @param list p_list:...
    */
    void set_list(deque<HDC*>* list);
    /**
    * @brief Performs deep copy of current node if recursively = 1. Performs shallow copy otherwise.
    *
    * @param h p_h:...
    * @param recursively p_recursively:...
    */
    void resize(HDC* h, int recursively = 0);
    /**
    * @brief Returns copy of current object.
    *
    * @param copy_arrays p_copy_arrays:...
    * @return HDC
    */
    HDC copy(int copy_arrays = 1);
    /**
    * @brief Inserts node to i-th slice of current node.
    *
    * @param i p_i:...
    * @param h p_h:...
    */
    void insert(size_t i, HDC* h);
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
    void append(HDC* h);
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
    * @brief Returns pointer to data of this node.
    *
    * @param T p_T: Desired data type.
    * @return T
    */
    template<typename T> T as() const
    {
        hdc_header_t header = get_header();
        if (header.type == HDC_STRUCT || header.type == HDC_LIST) {
            throw std::runtime_error("This is not a terminal node...");
        }
        DEBUG_STDOUT("as<"+get_type_str()+">()");
        if (!storage->has(uuid)) {
            throw HDCException("as(): Not found: "+std::string(uuid.c_str())+"\n");
        }
        return reinterpret_cast<T>(storage->get(uuid)+sizeof(hdc_header_t));
    }
    /**
    * @brief Returns scalar value.
    *
    * @param T p_T: Desired data type.
    * @return T
    */
    template<typename T> T as_scalar() const
    {
        hdc_header_t header = get_header();
        if (header.type == HDC_STRUCT || header.type == HDC_LIST) {
            throw std::runtime_error("This is not a terminal node...");
        }
        DEBUG_STDOUT("as<"+get_type_str()+">()");
        if (!storage->has(uuid)) {
            throw HDCException("as_scalar(): Not found: "+std::string(uuid.c_str())+"\n");
        }
        T result;
        memcpy(&result,storage->get(uuid)+sizeof(hdc_header_t),sizeof(T));
        return result;
        //return *reinterpret_cast<T>(storage->get(uuid)+sizeof(hdc_header_t));
    }
    /**
    * @brief Returns string. Needs to have separate function
    *
    * @return const std::__cxx11::string
    */
    const std::string as_string() const
    {
        hdc_header_t header = get_header();
        if (header.type == HDC_STRING) {
            std::string str(storage->get(uuid)+sizeof(hdc_header_t));
            return str;

        } else {
            std::cout << header.type << std::endl;
            std::ostringstream oss;
            oss << to_json(0) << "\n";
            return oss.str();
        }
    }
    const char* as_cstring() const
    {
        hdc_header_t header = get_header();
        if (header.type == HDC_STRING) {
            return static_cast<const char*>(get_buffer()+sizeof(hdc_header_t));
        } else {
            std::cout << header.type << std::endl;
            std::ostringstream oss;
            oss << to_json(0) << "\n";
            return oss.str().c_str();
        }
    }
    /**
    * @brief Serialization to JSON file.
    *
    * @param filename p_filename:...
    * @param mode p_mode:...
    */
    void to_json(string filename, int mode = 0) const;
    /**
    * @brief Serialization to Json::Value object.
    *
    * @param mode p_mode:...
    * @return Json::Value
    */
    Json::Value to_json(int mode = 0) const;
    /**
    * @brief Serialization to string object.
    *
    * @param mode p_mode:...
    * @return std::__cxx11::string
    */
    std::string to_json_string(int mode = 0) const;
    /**
    * @brief Dumps JSON to std::cout
    *
    */
    void dump() const;
    /**
    * @brief Serializes HDC to special json file
    *
    * @return const std::__cxx11::string
    */
    const std::string serialize() const;
    /**
    * @brief ...
    *
    * @param filename p_filename:...
    */
    void serialize(const std::string& filename) const;
    /**
    * @brief Returns void pointer to data.
    *
    * @return intptr_t
    */
    intptr_t as_void_ptr() const;
    /**
    * @brief Returns string representing data/node type.
    *
    * @return const char*
    */
    const char* get_type_str() const;
    /**
    * @brief Returns void pointer to data
    *
    * @return char*
    */
    char* get_data_ptr() const;
    /**
    * @brief Returns vector of keys of a struct node and empty vector otherwise.
    *
    * @return std::vector< std::__cxx11::string >
    */
    std::vector<std::string> keys() const;
    /**
    * @brief ...
    *
    * @return size_t
    */
    size_t childs_count() const;
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
    */
    void delete_data();
    /**
    * @brief ...
    *
    * @param signalName p_signalName:...
    * @param dataSource p_dataSource:...
    * @param withMetadata p_withMetadata:...
    * @return HDC
    */
    static HDC from_uda(const std::string& signalName, const std::string& dataSource, bool withMetadata = false);
    /**
    * @brief "static contructor" from void* HDC
    *
    * @param cpp_ptr p_cpp_ptr:...
    * @return HDC*
    */
    static HDC* new_HDC_from_cpp_ptr(intptr_t cpp_ptr);
    /**
    * @brief deserialize from storage
    *
    * @param filename p_filename:...
    * @return HDC*
    */
    static HDC deserialize_HDC_file(const std::string& filename);
    /**
    * @brief deserialize from storage
    *
    * @param filename p_filename:...
    * @return HDC*
    */
    static HDC deserialize_HDC_string(const std::string& filename);
    /**
    * @brief ...
    *
    * @param filename p_filename:...
    * @param datapath p_datapath:...
    * @return HDC
    */
    static HDC from_json(const std::string& filename, const std::string& datapath = "");
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
    * @param root p_root:...
    * @return HDC
    */
    static HDC json_to_HDC(const ::Json::Value& root);
    /**
    * @brief ...
    *
    * @param json_string p_json_string:...
    * @return HDC
    */
    static HDC from_json_string(const std::string& json_string);
    /**
    * @brief ...
    *
    * @param filename p_filename:...
    * @param dataset_name p_dataset_name:...
    */
    void to_hdf5(std::string filename, std::string dataset_name = "data");
    /**
    * @brief ...
    *
    * @param filename p_filename:...
    * @param dataset_name p_dataset_name:...
    * @return HDC
    */
    static HDC from_hdf5(const std::string& filename, const std::string& dataset_name = "/data");
    /**
    * @brief ...
    *
    * @param filename p_filename:...
    * @param dataset_name p_dataset_name:...
    * @return HDC*
    */
    static HDC* from_hdf5_ptr(const std::string& filename, const std::string& dataset_name = "/data");
    /**
    * @brief ...
    *
    * @param data_object p_data_object:...
    * @param data_source p_data_source:...
    * @return HDC
    */
    static HDC uda2HDC(const std::string& data_object, const std::string& data_source);
    /**
    * @brief ...
    *
    * @param str p_str:...
    * @param datapath p_datapath:...
    * @return HDC
    */
    static HDC load(const std::string& str, const std::string& datapath="");
    /**
    * @brief ...
    *
    * @return HDCStorage*
    */
    HDCStorage* get_storage() const {return this->storage; };
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
};

#endif // HDC_HPP
