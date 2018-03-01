KNOWN PROBLEMS
==============
- IT IS STILL JUST A TOY
- Each string in node address has to be shorter than 1024 chars. (or you need to preallocate larger buffer)
- insert_slice() needs preallocated buffer if multiple intems are saved
- whole thing lacks locking - we need to add some (at least to make MDBM happy)
- saving multiple nodes needs preallocated buffer
- some small blocks of memory may be lost
