#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

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

typedef enum {
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
    ERROR_ID    = HDC_ERROR,
    Internal_ForceMyEnumIntSize = ULONG_MAX // enum : unsigned int is not compatible with C, so we will do this and hope that it will be ok.
} TypeID;

#define HDCDefault        0lu
#define HDCFortranOrder   1lu
#define HDCReadOnly       2lu
#define HDCExternal       4lu
#define HDCChildrenInitialized 8lu

typedef unsigned long Flags;

struct header_t {
    size_t buffer_size;
    size_t data_size;
    size_t type;
    size_t flags;
    size_t shape[10];
    size_t ndim;
    size_t pad; // Padding
};
//} __attribute__((packed));

#define HDC_NODE_SIZE_DEFAULT 4096 // we can try less here 3072 works, 2048 does not
#define HDC_NODE_SIZE_INCREMENT 1024


//TODO Specify offsets in header, possibly make header of dynamic length.
//TODO support external data
//TODO HDF5 1) as serialization, 2) as backend

struct hdc_t;

#endif
