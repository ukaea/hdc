#ifndef HDC_MAP_H
#define HDC_MAP_H


#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/shared_ptr.hpp>


class HDC;


using namespace boost::multi_index;
using namespace std;

struct record
{
    record(string _key, HDC* _node): key(_key), node(_node) {};
    string key;
    HDC* node;
};

struct change_node
{
    change_node(HDC*& new_node):new_node(new_node){}
    void operator()(record& r)
    {
        r.node=new_node;
    }
private:
    HDC* new_node;
};

struct by_index{};
struct by_key{};
struct by_sequence{};

typedef multi_index_container<
    record,
    indexed_by<
        hashed_non_unique<
            tag<by_key>,
            member<
                record, string, &record::key
            >
        >,
        random_access<
            tag<by_index>
        >,
        sequenced<
            tag<by_sequence>
        >
    >
> map_t;

#endif