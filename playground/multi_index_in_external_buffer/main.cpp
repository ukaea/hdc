/* This Example shows how to use boost::multi_index with hashed_non_unique and random_access indices allocated in simple char* buffer.
 * The main purpose of such a object is to efficienty map hierarchy of CPO-like object (nested structure) into key-value store,
 * which is for now one of our principal concept in HDC.
 * 
 * We use "Child References model" from https://docs.mongodb.com/manual/tutorial/model-tree-structures/ for the mapping.
 * Each non-terminal node can contain references to other nodes. In this example, such references are stored using already
 * mentioned boost::multi_index container, which mimics the behaviour of OrderedDict in Python.
 * 
 * For accessing children from different processes, one could use serialization. However such approach would be inefficient, so we
 * propose to combine our multi_index container with boost::interprocess::managed_external_buffer, which is basically set of
 * offset pointers, segment managment and allocator. Over the buffer we create segment manager, which is then responsible for creating
 * or destroying objects in buffer.
 * 
 * Using this approach one has to ensure, that all keys and values are stored in the same buffer. That implies use of boost::basic_string<... our_allocator>
 * instead of std::string, however default template parameters imply use of the same key type in find() method, which would allocate string within the buffer every time we look up for something by the string key. In order to avoid this flaw, we have to implement our own hash and equal operators.
 * 
 * The following code is a case study and test of all things in more simple environment. Not all should be perfectly correct here, so be aware...
 * 
 * Further reading:
 * 
 * http://stackoverflow.com/questions/20137230/boostmulti-index-container-equal-range-values
 * http://stackoverflow.com/questions/11150014/boostproperty-tree-compile-error-error-c2061-syntax-error-identifier-p
 * http://stackoverflow.com/questions/26474577/using-boost-multi-index-like-relational-db
 * http://stackoverflow.com/questions/14595770/gdb-lookup-type-returns-invalid-type-with-boostmulti-index-container
 * http://stackoverflow.com/questions/27328835/want-to-efficiently-overcome-mismatch-between-key-types-in-a-map-in-boost-interp/27330042#27330042
 * http://stackoverflow.com/questions/7447976/boostinterprocessbasic-string-as-stdstring
 * http://stackoverflow.com/search?q=char*+basic_string+boost+multi_index
 * http://www.boost.org/doc/libs/1_61_0/libs/multi_index/doc/examples.html#example8
 * http://www.boost.org/doc/libs/1_61_0/libs/multi_index/example/ip_allocator.cpp
 * http://boost.2283326.n4.nabble.com/Boost-Interprocess-accessing-an-unordered-map-in-shared-memory-td4644023.html
 * http://boost.2283326.n4.nabble.com/multi-index-interprocess-find-key-must-be-allocated-in-shared-memory-td2581182.html
 * http://www.boost.org/doc/libs/1_61_0/doc/html/intrusive/advanced_lookups_insertions.html
 * http://stackoverflow.com/questions/23443890/using-boost-unordered-hash-map-with-char-key
 * 
 */

#include <iostream>

/* Interprocess allocator stuff */

#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

namespace bip=boost::interprocess;

/* Map stuff */

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/functional/hash.hpp>

using namespace boost::multi_index;
using namespace std;

// Allocator definition
typedef bip::managed_external_buffer::allocator<char>::type char_allocator;

// Strings have to live inside buffer
typedef bip::basic_string<
  char,std::char_traits<char>,
  char_allocator
> shared_string;

// Hashing class - we want to lookup using const char* and std::string
struct shared_string_hash {
    std::size_t operator ()(const shared_string& str) const {
        boost::hash<std::string> hasher;
        size_t hashed = hasher(str.c_str());
//         std::cout << "shared_string: " << str  << hashed << "\n";getchar();
        return hashed;
    }
    std::size_t operator ()(const char* str) const {
        boost::hash<std::string> hasher;
        size_t hashed = hasher(str);
//         std::cout << "char*: " << str  << hasher(str) << "\n";getchar();
        return hashed;
    }
};

// Equal predicate -- replacement fot std::equal_to, same reasom as hash
struct shared_string_equal {
    template <class T, class U>
    bool compare(const T& lhs, const U& rhs) const {
        return lhs == rhs;
    }
    template <class T, class U>
    bool operator()(const T& lhs, const U& rhs) const {
        return compare(lhs,rhs);
    }
};

// key specifiers
struct by_index{};
struct by_key{};
struct by_sequence{};

// We want to store (key,address) pairs
struct record
{
    record(const char* _key, const char* _address, const char_allocator &a): key(_key,a), address(_address,a) {};
    shared_string key;
    shared_string address;
};
// Modifying functor
struct change_node
{
    change_node(shared_string& new_address):new_address(new_address){}
    void operator()(record& r)
    {
        r.address=new_address;
    }
private:
    shared_string new_address;
};

// Finally, our children nodes storage
typedef multi_index_container<
    record,
    indexed_by<
        hashed_non_unique<
            tag<by_key>,
            member<
                record, shared_string, &record::key
            >,
            //boost::hash<shared_string>, // This would be default hasher
            shared_string_hash,
            //std::equal_to<shared_string> // This would be default Pred
            shared_string_equal
        >,
        random_access<
            tag<by_index>
        >,
        sequenced<
            tag<by_sequence>
        >
    >,
    bip::managed_external_buffer::allocator<record>::type // our allocator
> map_t;

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;

    // Create buffer with known size
    size_t memsize = 1300;
    char* buffer = new char[memsize];
    
    // initialize segment manager
    bip::managed_external_buffer* segment = new bip::managed_external_buffer(bip::create_only, buffer, memsize);
    
    // construct children map inside the buffer
    map_t *children = segment->construct<map_t>("a")(map_t::ctor_args_list(),segment->get_allocator<record>());
    
    // store some small values
    children->insert(record("bbb","BBB",segment->get_allocator<char>())); //see below for more compact notation
    children->insert(record("ccc","CCC",segment->get_allocator<char>()));
    
    int redo = 1; // success flag - ensure to insert the record just once
    while (redo) {
        // insert the key if it fits in
        try {
            cout << "Size: " << segment->get_size() << "\n";
            children->insert(record("aaaaaaaaaaa","AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",segment->get_allocator<char>()));
            redo = 0;
        }
        // if there is no space, increase the buffer size
        catch (bip::bad_alloc &) {
            std::cout << "Buffer too small. Resizing." << std::endl;

            // Create new larger buffer
            memsize += 50;
            char* larger_buffer = new char[memsize];
            bip::managed_external_buffer* larger_segment = new bip::managed_external_buffer(bip::create_only, larger_buffer, memsize);
            
            // Construct new map. It seems impossible to increase the size of managed segment
            map_t* larger_children = larger_segment->construct<map_t>("a")(map_t::ctor_args_list(),larger_segment->get_allocator<record>());
            
            // copy all items to new larger storage
            for (map_t::iterator it = children->begin(); it != children->end(); ++it) {
                larger_children->insert(*it);
            }

            // delete old buffer
            delete segment;
            delete[] buffer;

            //change pointers
            buffer = larger_buffer;
            segment = larger_segment;
            children = larger_children;

            //indicate sucess to end the enlarging loop
            redo = 1;
        }
    }
    std::cout << "Done" << std::endl;


    // Copy buffer somewhere, try to access its data
    char* buffer2 = new char[memsize];
    std::copy(buffer,buffer+memsize,buffer2);
    
    // wake up the segment manager
    bip::managed_external_buffer segment2(bip::open_only, buffer2, memsize);

    // obtain the allocator
    bip::managed_external_buffer::allocator<char>::type ca = segment2.get_allocator<char>();

    // try to find map - we should try-catch this
    map_t *children2 = segment2.find<map_t>("a").first;
    // exit if not found
    if (!children2) return 1;
    std::cout << children2->empty() << std::endl;

    // Ensure, that the new storage contains the same data
    std::cout << "Lookup start" << std::endl;
    std::cout << children2->find("aaaaaaaaaaa")->address << std::endl;
    std::cout << children2->find("bbb")->address << std::endl;
    std::cout << children2->find("ccc")->address << std::endl;
    std::cout << "Lookup end" << std::endl;
    
    // Clean up
    delete[] buffer;
    delete[] buffer2;

    return 0;
}
