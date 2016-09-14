What should be done before merging the testing branch:

- "serialize" whole tree into KV store, not just terminal nodes. See https://docs.mongodb.com/manual/applications/data-models-tree-structures/ -- we should want the easiest possible solution for this. Possibilities are:
    - Store children in dom object, possibly json, serialization into KV will be quite efficient. Question is, how big arrays of structs we will have to deal with.
        - Pros: get rid of Boost, easy
        - Cons: slow
    - Keep Boost::multi_index and searialize on demand -- will need external synchronization I want to avoid for now.
    - Keep Boost::multi_index and searialize on change of children -- possibly not a big deal.
    - Use another mechanism for tree synchonization (I don't know...)
    - it will be necessary to track parrent node in in every child in order to remove child from parrent on its destruction
- create mechanism to switch storage at runtime


Other things to do:
- dynamic buffer headers
- buffer header versioning (or just add data offset st the beginning)
- revise handling of multidim arrays in C / Fortran
    - storage will be allways contiguous in memory =>
        - provide indexing macro for C
        - fortran seems to be able to build array of any shape around the buffer
- revise Python support
    - start with pybind11, advanced functions in Python
