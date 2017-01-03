#ifndef HDC_MAP_H
#define HDC_MAP_H

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

#include <boost/aligned_storage.hpp>

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
    record(std::string& _key, std::string& _address, const char_allocator &a): key(_key.c_str(),a), address(_address.c_str(),a) {};
    record(std::string& _key, const char* _address, const char_allocator &a): key(_key.c_str(),a), address(_address,a) {};
    record(const char * _key, std::string& _address, const char_allocator &a): key(_key,a), address(_address.c_str(),a) {};

    shared_string key;
    shared_string address;

    friend std::ostream& operator<<(std::ostream& os,const record& r)
    {
        os << r.key << " " << r.address << std::endl;
        return os;
    }
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

#endif
