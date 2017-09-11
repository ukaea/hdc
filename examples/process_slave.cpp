#include <iostream>
#include "hdc.hpp"

#include <vector>
#include <string>
using namespace std;


int main(int argc, char *argv[]) {
    string filename(argv[1]);
    cout << filename << "\n";
    // Create new HDC tree
    HDC* tree = deserialize_HDC_file(filename);
    tree->dump();

    return 0;
}
