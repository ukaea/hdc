#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <iostream>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/shared_ptr.hpp>

#define    HDC_MAX_DIMS  10
#define    HDC_LIST_INIT_CAPACITY 10
#define    HDC_LIST_RESIZE_STEP 10

#define    HDC_EMPTY      0
#define    HDC_STRUCT     1
#define    HDC_LIST       2
#define    HDC_INT8       3
#define    HDC_INT16      4
#define    HDC_INT32      5
#define    HDC_INT64      6
#define    HDC_UINT8      7
#define    HDC_UINT16     8
#define    HDC_UINT32     9
#define    HDC_UINT64    11
#define    HDC_FLOAT     12
#define    HDC_DOUBLE    13
#define    HDC_STRING    14
#define    HDC_BOOL      15
#define    HDC_ERROR     16

using namespace boost::multi_index;
using namespace std;

typedef enum : unsigned long { // padding by C++11
    EMPTY_ID    = HDC_EMPTY,
    STRUCT_ID   = HDC_STRUCT,
    LIST_ID     = HDC_LIST,
    INT8_ID     = HDC_INT8,
    INT16_ID    = HDC_INT16,
    INT32_ID    = HDC_INT32,
    INT64_ID    = HDC_INT64,
    UINT8_ID    = HDC_UINT8,
    UINT16_ID   = HDC_UINT16,
    UINT32_ID   = HDC_UINT32,
    UINT64_ID   = HDC_UINT64,
    FLOAT_ID    = HDC_FLOAT,
    DOUBLE_ID   = HDC_DOUBLE,
    STRING_ID   = HDC_STRING,
    BOOL_ID     = HDC_BOOL,
    ERROR_ID    = HDC_ERROR
} TypeID;

#define HDCDefault        0lu
#define HDCFortranOrder   1lu
#define HDCReadOnly       2lu
#define HDCExternal       4lu

typedef unsigned long Flags;


#define HDC_TYPE_POS     0
#define HDC_FLAGS_POS    sizeof(TypeID)
#define HDC_SHAPE_POS    HDC_FLAGS_POS + sizeof(Flags)
#define HDC_DATA_POS     HDC_SHAPE_POS + HDC_MAX_DIMS * sizeof(size_t)

class HDC;

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

struct hdc_t;

#endif