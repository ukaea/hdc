HDC - Hierarchical Dynamic Containers                         {#mainpage}
=====================================

HDC is tiny library for exchanging hierarchical data (arrays of structures) in shared memory between multiple programming languages, currently supporting C, C++, Python and Fortran.

As of May 2016 it is in a preview state - it works, but many work is still need to be done.

Building instructions
=====================

Prerequisites
-------------

To build HDC, you will need:
- c++14 compliant compiler (at least gcc-4.9)
- libDyND library
- Python 2.7 or any Python 3 (tested only with 3.4 and 3.5)
- CMake >= 2.8.11
- Doxygen for documentation building

Building libDyND
----------------
For complete guide please follow instructions on [libDyND project page](https://github.com/libdynd/libdynd)
```
git clone --recursive https://github.com/libdynd/libdynd.git
# Checkout the last tested working commit
git checkout 6e81192606875a9408f19a997854a46959f48cb4
# Go to directory
cd libdynd
# Create build directory
mkdir build
cd build
# make
cmake ..
make
make install
```

Building HDC
------------
```
cd hdc_new
mkdir build cd build
cmake ..
make
# Optionally build documentation
make doc
```

Basic ideas
===========
- The hierarchical structure is organized as tree.
- Subtrees (Nodes) can be accessed by path similarily to file system paths like "aaa/bbb"
- Each node can be one of several types:
  + Empty node - this is the initial state of node. Empty node does not store any data and does not have any children. By adding subnode, slice or data it's type is automaticaly changed to another type.
  + Structure node - the node has at least one children indexed by string path. It can only store subtrees indexed by path.
  + Array node - the node has at least one children indexed by integer. It can only store subtrees indexed by integer.
  + Data node - it only can be terminal node, it stores some data, currently in DyND array object.


Examples
========
Very simple examples follow. More complete examples can be found in src/test folder

Example in C++
--------------
```
// Create empty tree
HDC* tree = new HDC();
// Add some node
tree->add_child("aaa/bbb/ccc",new HDC());
// Store data to terminal node
int8_t array[4] = {1,2,3,4};
long int shape[1];
shape[0] = 4;
tree->set_data<int8_t>("aaa/bbb/ccc",1,shape,(void*)array);
// Get subtree
HDC* subtree = tree->get_child("aaa/bbb");
// Get data from subtree
int8_t* array2 = subtree->as<int8_t>("ccc");
// Delete tree
delete tree;
```

Example in C
------------
```
// Create empty tree
struct hdc_t* tree = hdc_new_empty();
// Add some node
hdc_add_child(tree,"aaa/bbb/ccc",hdc_new_empty());
// Store data to terminal node
int8_t array[4] = {1,2,3,4};
long int shape[1];
shape[0] = 4;
hdc_set_data_int8(data,1,shape,(void*)array);
// Get subtree
struct hdc_t* subtree = hdc_get_child(tree,"aaa/bbb");
// Get data from subtree
int8_t* array2 = (int8_t*)hdc_as_int_1d(subtree,"ccc");
// Delete tree
hdc_delete(tree);
```

Example in Fortran
------------------
```
type(hdc_t) :: tree, subtree
integer(kind=c_int8_t), dimension(1:4) :: array, array2
integer, dimension(1:1) :: shape_
! Create empty tree
tree = hdc_new_empty()
! Add some node
call hdc_add_child(tree,"aaa/bbb/ccc",hdc_new_empty())
! Store data to terminal node
array = (/1,2,3,4/)
shape_(1) = 4
call hdc_set_data(tree,1,shape,array)
! Get subtree
subtree = hdc_get_child(tree,"aaa/bbb")
! Get data from subtree
array2 = hdc_get_int8_1d(subtree,"ccc")
! Or
call hdc_get(subtree,"ccc",array2)
```