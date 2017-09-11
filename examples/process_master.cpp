#include <iostream>
#include "hdc.hpp"

#include <vector>
#include <string>
using namespace std;


int main() {
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
    size_t shape[1];
    shape[0] = 4;

    // Add data to a single node
    HDC data;
    data.set_data(1,shape,array);
    // Add data to a subtree
    tree.set_data("aaa/bbb/ccc",1,shape,array);

    // Ask for some data details, use subtree to shorten the path
    int32_t ndim2 = node.get_ndim();
    size_t* shape2 = node.get_shape();
    // Get data back from tree
    int32_t* array2 = node.as<int32_t*>();
    tree.serialize("pokus.json");
    cout << "opening slave\n";
    system("./process_slave pokus.json");
    cout << "done\n";
    return 0;
}
