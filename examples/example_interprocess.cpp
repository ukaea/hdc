#include <iostream>
#include "hdc.hpp"

#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/algorithm/string/erase.hpp>

using namespace std;
using namespace boost::interprocess;

int main(int argc, char *argv[]) {

    if (argc == 1) {
        cout << "master\n";
        //Remove shared memory on construction and destruction
        struct shm_remove
        {
            shm_remove() { shared_memory_object::remove("MySharedMemory"); }
            ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
        } remover;
        HDC::init("mdbm","{\"filename\": \"/tmp/db.mdbm\", \"persistent\": true}");
        //HDC_init("mdbm","{\"filename\": \"/tmp/db.mdbm\"}");
        // Create new HDC tree
        HDC tree;
        // Add some children
        HDC dummy1;
        tree.add_child("aaa/bbb/ccc",dummy1);
        HDC dummy2;
        tree.add_child("aaa/bbb/eee",dummy2);
        HDC dummy3;
        tree.add_child("aaa/eee/bbb",dummy3);
        HDC dummy4;
        tree.add_child("eee",dummy4);
        HDC dummy5;
        tree.add_child("bbb",dummy5);
        HDC dummy6;
        tree.add_child("ccc",dummy6);

        // Get subtree
        HDC subtree = tree.get("aaa/bbb");
        // Get node
        HDC node = subtree.get("ccc");

        // Delete subtree
        tree.delete_child("aaa/eee");

        // Prepare some data
        int32_t array[4] = {7,2,3,4};
        std::vector<size_t> shape = {4};

        // Add data to a single node
        HDC data;
        data.set_data(1,shape,array);
        // Add data to a subtree
        tree["aaa/bbb/ccc"].set_data(1,shape,array);

        // Ask for some data details, use subtree to shorten the path
//        int32_t rank2 = node.get_rank();
//        size_t* shape2 = node.get_shape();
        // Get data back from tree
//        int32_t* array2 = node.as<int32_t*>();
        // get settings string
        std::string settings = tree.serialize();
        boost::algorithm::erase_all(settings,"\\"); // hack to remove wrongly escaped path

        shared_memory_object shm (create_only, "MySharedMemory", read_write);
        shm.truncate(65536);
        mapped_region region(shm, read_write);
        std::memset(region.get_address(), 0, region.get_size());
        std::memcpy(region.get_address(), settings.c_str(),settings.length());

        //Launch child process
        std::string s(argv[0]); s += " child ";
        if(0 != std::system(s.c_str()))
            return 1;
    } else {
        shared_memory_object shm (open_only, "MySharedMemory", read_only);
        mapped_region region(shm, read_only);
        char *mem = static_cast<char*>(region.get_address());
        std::string str(mem);
        HDC tree = HDC::deserialize_HDC_string(str);
        tree.dump();
    }
    return 0;
}
