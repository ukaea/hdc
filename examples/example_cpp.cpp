#include <iostream>
#include "hdc.hpp"

#include <vector>

using namespace std;


int main() {
    // Create new HDC tree
    HDC* tree = new HDC();
//    tree->set_type((TypeID)HDC_STRUCT);
    // Add some children
//     tree->add_child("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",new HDC());
    tree->add_child("aaa/bbb/ccc",new HDC());
    tree->add_child("aaa/bbb/eee",new HDC());
    tree->add_child("aaa/eee/bbb",new HDC());
    tree->add_child("eee",new HDC());
    tree->add_child("bbb",new HDC());
    tree->add_child("ccc",new HDC());
    
    tree->grow(1111);
    for (auto it : tree->keys()) cout << "key: " << it << endl;
    // Get subtree
    HDC* subtree = tree->get("aaa/bbb");
    // Get node
    HDC* node = subtree->get("ccc");

    // Ask whether child exists
    cout << "has_child: " << tree->has_child("aaa/bbb/ccc") << endl;

    // Delete subtree
    tree->delete_child("aaa/eee");

    // Prepare some data
    int32_t array[4] = {7,2,3,4};
    size_t shape[1];
    shape[0] = 4;

    // Add data to a single node
    HDC* data = new HDC();
    data->set_data(1,shape,array);

    // Add data to a subtree
    tree->set_data("aaa/bbb/ccc",1,shape,array);

    // Ask for some data details, use subtree to shorten the path
    int32_t ndim2 = node->get_ndim();
    size_t* shape2 = node->get_shape();
    cout << "Dimension: " << (int)ndim2 << endl << "Shape: ";
    for (int i=0; i<ndim2; i++) cout << (int)shape2[i] << " ";
    cout << endl;
    cout << "dtype: " << node->get_type_str() << endl;
    // Get data back from tree
    int32_t* array2 = node->as<int32_t*>();
    cout << "Data: ";
    for (int i=0; i<shape2[0]; i++) cout << array2[i] << " ";
    cout << endl;
    //Serialize data to JSON
    tree->to_json("tree.txt",0);
    // On screen
    tree->dump();
    return 0;
}
