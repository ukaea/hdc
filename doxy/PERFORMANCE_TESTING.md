Some HDC benchmarking results
=============================

We use [Google Benchmark](https://github.com/google/benchmark) library for benchmarking. Simirarly to the unit tests, the neccessary library is automatically downloaded and installed using cmake. The benchmark executable can be found and run in cmake `cmake_build_dir/bin/benchmark_hdc`, sources can be found in `benchmark` directory.

Two aspects have been tested separately:

  - Data manipulation
  - Tree manipulation

All presented results were run on Intel Core i5-4310U with Dual Channel Memory configuration.

Data manipulation
-----------------

  - Setting data - tests `BM_HDC_SetData`, `BM_memcpy` - Comparison of `memcpy` function and `hdc::set` shows, that HDC is 2-3 times slower than `memcpy` - allowing throughput around 5.33GB/s compared to 11.32GB/s of `memcpy`
  - Setting data (zero copy version) `BM_HDC_SetDataZeroCopy` -  There is an option to set data without copying, but it is not set by default because of garbage collecting mechanisms in Fortran and Python. It shows, that setting pointer takes ~400ns
  - Getting the data pointer - `BM_HDC_GetData` takes 13ns per item.

Node manipulation
-----------------
    
  - Calling of constuctor and destructor of an empty HDC `BM_HDC_CreationAndDeletion` takes on average 196ns - it is 4.87241M items/s
  - Adding childs (`BM_HDC_AddChild`) scales nearly linearly - 376.471k items/s for 32K items in `std::unordered_map`.
  - `BM_HDC_AddChildPathDepth` shows how is the accessing time influenced by the depth of recursion - each single level of recursion (i.e. slash in path) makes on average 2us. It is possible to get subtree to minimize this delay when needed.
  - In `BM_HDC_AddChildPathLength` the inluence of key length is tested - we got 3us for single level key made of 1024 characters and 7.4us for key with 32k characters
  - In `BM_HDC_GetSliceMultipleItems` the influence of number of children in single level on access time has been tested. There is none - time is the same for 1024 and 32k nodes on the same level.
  - The last tests (`BM_HDC_AppendSlice`, `BM_HDC_AddChild`, `BM_HDC_GetSliceMultipleItems`, `BM_HDC_GetChildMultipleItems`) shows differences between appending slice/addin child to tree and getting slice/child from tree. Slices operations seem to be more than 12x faster. Size of map/list does not affect speed of adding.
