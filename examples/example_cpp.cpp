#include <iostream>
#include "hdc.hpp"
#include <vector>
#include <string>

using namespace std;

int main()
{
    HDC::init();
    // Create new HDC tree
    HDC tree;
    // Add some children - one can use either bracket operator or add_child() method
    HDC dummy1;
    tree["aaa/bbb/ccc"] = dummy1;
    HDC dummy2;
    tree["aaa/bbb/eee"] = dummy2;
    HDC dummy3;
    tree["aaa/eee/bbb"] =  dummy3;
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
    HDC node = subtree["ccc"];

    // Ask whether child exists
    cout << "exists: " << tree.exists("aaa/bbb/ccc") << endl;

    // Delete subtree
    tree.delete_child("aaa/eee");

    // Prepare some data
    int32_t array[4] = { 7, 2, 3, 4 };
    std::vector<size_t> shape = {4};
    // Add data to a single node
    HDC data;
    data.set_data(shape, array);
    // Add data to a subtree
    //tree.get_or_create("aaa/bbb/ccc").set_data(1, shape, array);
    tree["aaa/bbb/ccc"].set_data(shape, array);
    // Ask for some data details, use subtree to shorten the path
    int32_t rank2 = node.get_rank();
    auto shape2 = node.get_shape();
    cout << "Dimension: " << (int)rank2 << endl << "Shape: ";
    for (int i = 0; i < rank2; i++) cout << shape2[i] << " ";
    cout << endl;
    cout << "dtype: " << node.get_type_str() << endl;
    // Get data back from tree
    int32_t* array2 = node.as<int32_t>();
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
    HDC::destroy();

    return 0;
}
