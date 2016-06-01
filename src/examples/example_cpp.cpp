#include <iostream>
#include "hdc.hpp"

#include <vector>

using namespace std;


int main(int argc, char **argv) {

    // Create new HDC tree
    HDC* tree = new HDC();

    // Add some children
    tree->add_child("aaa/bbb/ccc",new HDC());
    tree->add_child("aaa/bbb/eee",new HDC());
    tree->add_child("bbb/eee/aaa",new HDC());

    // Get subtree
    HDC* subtree = tree->get_child("aaa/bbb");

    // Ask whether child exists
    cout << "has_child: " << tree->has_child("aaa/bbb/ccc") << endl;

    // Delete subtree
    tree->delete_child("aaa/eee");

    // Prepare some data
    int32_t array[4] = {7,2,3,4};
    long int shape[1];
    shape[0] = 4;

    // Add data to a single node
    HDC* data = new HDC();
    data->set_data<int32_t>(1,shape,(void*)array);

    // Add data to a subtree
    tree->set_data<int32_t>("aaa/bbb/ccc",1,shape,(void*)array);

    // Ask on some data details, use subtree to shorten the path
    int32_t ndim2 = subtree->get_ndim("ccc");
    long int* shape2 = subtree->get_shape("ccc");
    cout << "Dimension: " << (int)ndim2 << endl << "Shape: ";
    for (int i=0; i<ndim2; i++) cout << (int)shape2[i] << " ";
    cout << endl;
    cout << "dtype: " << subtree->get_type_str("ccc") << endl;

    // Get data back from tree
    int32_t* array2 = subtree->as<int32_t*>("ccc");
    cout << "Data: ";
    for (int i=0; i<shape2[0]; i++) cout << array2[i] << " ";
    cout << endl;

    //Serialize data to JSON
    tree->to_json("tree.txt",0);
    // On screen
    tree->dump();
    return 0;
}
