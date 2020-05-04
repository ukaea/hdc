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

/* Map stuff */

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/functional/hash.hpp>

#include <boost/aligned_storage.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>

// Allocator definition
typedef boost::interprocess::managed_external_buffer::allocator<char>::type char_allocator;

// Strings have to live inside buffer
typedef boost::interprocess::basic_string<
        char, std::char_traits<char>,
        char_allocator
> shared_string;

// Hashing class - we want to lookup using const char* and std::string
struct shared_string_hash {
    std::size_t operator()(const shared_string& str) const
    {
        boost::hash<std::string> hasher;
        size_t hashed = hasher(str.c_str());
        return hashed;
    }

    std::size_t operator()(const char* str) const
    {
        boost::hash<std::string> hasher;
        size_t hashed = hasher(str);
        return hashed;
    }
};

// Equal predicate -- replacement for std::equal_to, same reason as hash
struct shared_string_equal {
    template <class T, class U>
    static bool compare(const T& lhs, const U& rhs)
    {
        return lhs == rhs;
    }

    template <class T, class U>
    bool operator()(const T& lhs, const U& rhs) const
    {
        return compare(lhs, rhs);
    }
};

// key specifiers
struct by_index {
};
struct by_key {
};
struct by_sequence {
};

// We want to store (key,address) pairs
struct record {
    record(const char* _key, const char* _address, const char_allocator& a) : key(_key, a), address(boost::lexical_cast<boost::uuids::uuid>(_address))
    {};

    record(std::string& _key, std::string& _address, const char_allocator& a) : key(_key.c_str(), a),
                                                                                address(boost::lexical_cast<boost::uuids::uuid>(_address))
    {};

    record(std::string& _key, const char* _address, const char_allocator& a) : key(_key.c_str(), a),
                                                                               address(boost::lexical_cast<boost::uuids::uuid>(_address))
    {};

    record(const char* _key, std::string& _address, const char_allocator& a) : key(_key, a),
                                                                               address(boost::lexical_cast<boost::uuids::uuid>(_address))
    {};
    record(std::string& _key, boost::uuids::uuid _address, const char_allocator& a) : key(_key.c_str(), a),
                                                                               address(_address)
    {};

    record(const char* _key, boost::uuids::uuid _address, const char_allocator& a) : key(_key, a),
                                                                               address(_address)
    {};
    shared_string key;
    boost::uuids::uuid address;

    friend std::ostream& operator<<(std::ostream& os, const record& r)
    {
        os << r.key << " " << r.address << std::endl;
        return os;
    }
};

// Modifying functor
struct change_node {
    explicit change_node(shared_string& new_address) : new_address(new_address)
    {}

    void operator()(record& r)
    {
        r.address = boost::lexical_cast<boost::uuids::uuid>(new_address);
    }

private:
    shared_string new_address;
};

// Finally, our children nodes storage
using hdc_map_t = boost::multi_index::multi_index_container<
        record,
        boost::multi_index::indexed_by<
                boost::multi_index::hashed_non_unique<
                        boost::multi_index::tag<by_key>,
                        boost::multi_index::member<
                                record, shared_string, &record::key
                        >,
                        //boost::hash<shared_string>, // This would be default hasher
                        shared_string_hash,
                        //std::equal_to<shared_string> // This would be default Pred
                        shared_string_equal
                >,
                boost::multi_index::random_access<
                        boost::multi_index::tag<by_index>
                >
        >,
        boost::interprocess::managed_external_buffer::allocator<record>::type // our allocator
>;

#endif
