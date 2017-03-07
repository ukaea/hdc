KNOWN PROBLEMS
==============

- Each string in node address has to be shorter than 1024 chars. (or you need to preallocate larger buffer)
- insert_slice() nedds preallocated buffer if multiple intems are saved
