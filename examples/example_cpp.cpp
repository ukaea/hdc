#include <iostream>
#include "hdc.hpp"
#include <vector>
#include <string>

using namespace std;

int main(int argc, const char* argv[])
{
    HDC::parse_cmdline(argc, argv);
    HDC::init();
    // Create new HDC tree
    HDC tree;
    // Add some children
    HDC dummy1;
    tree.add_child("aaa/bbb/ccc", dummy1);
    HDC dummy2;
    tree.add_child("aaa/bbb/eee", dummy2);
    HDC dummy3;
    tree.add_child("aaa/eee/bbb", dummy3);
    HDC dummy4;
    tree.add_child("eee", dummy4);
    HDC dummy5;
    tree.add_child("bbb", dummy5);
    HDC dummy6;
    tree.add_child("ccc", dummy6);

    for (auto it : tree.keys()) cout << "key: " << it << endl;
    // Get subtree
    HDC subtree = tree.get("aaa/bbb");
    // Get node
    HDC node = subtree.get("ccc");

    // Ask whether child exists
    cout << "exists: " << tree.exists("aaa/bbb/ccc") << endl;

    // Delete subtree
    tree.delete_child("aaa/eee");

    // Prepare some data
    int32_t array[4] = { 7, 2, 3, 4 };
    size_t shape[1];
    shape[0] = 4;

    // Add data to a single node
    HDC data;
    data.set_data(1, shape, array);
    // Add data to a subtree
    tree.set_data("aaa/bbb/ccc", 1, shape, array);

    // Ask for some data details, use subtree to shorten the path
    int32_t rank2 = node.get_rank();
    size_t* shape2 = node.get_shape();
    cout << "Dimension: " << (int)rank2 << endl << "Shape: ";
    for (int i = 0; i < rank2; i++) cout << (int)shape2[i] << " ";
    cout << endl;
    cout << "dtype: " << node.get_type_str() << endl;
    // Get data back from tree
    int32_t* array2 = node.as<int32_t*>();
    cout << "Data: ";
    for (size_t i = 0; i < shape2[0]; i++) cout << array2[i] << " ";
    cout << endl;

    //Serialize data to JSON
   tree.to_json("tree.txt",0);
    // On screen
    tree.dump();
    tree.serialize("pokus.json");

//Serialize data to HDF5
#ifdef _USE_HDF5
    tree.to_hdf5("tree.h5");
    HDC hhh = HDC::from_hdf5("tree.h5","/data");
    hhh.dump();
#endif


    std::cout << "---------------------------- overloaded operators start------------------------------\n";
    tree["aaa/bbb/ccc"].dump(); // works
    int32_t test[4] = { 100, 200, 300, 400 };
    HDC htest;
    htest.set_data(1, shape, test);
    tree["aaa/bbb/ccc"] = htest; // TODO
    tree["aaa/bbb/ccc"].dump();
    std::cout << "---------------------------- overloaded operators end --------------------------------\n";

    return 0;
}
