#include <iostream>
#include "hdc.h"

using namespace std;


int main(int argc, char **argv) {
    cout << "Hello, world!" << endl;
    
    string s = "aaa/bbb/ccc/ddd";
    stringstream ss(s);
    string first;
    getline(ss,first,'/');
    cout << first << "   :   " << s  << endl;
    
    vector<string> vs;
    split(s,'/',vs);
    while(!vs.empty()) {
        cout << vs[0] << endl; 
        vs.erase(vs.begin());
    }
        
    hdc* tree = new hdc();
    // test add
    tree->add_child("aaa/bbb/ccc",new hdc());
    tree->add_child("aaa/bbb/eee",new hdc());
    tree->add_child("bbb/eee/aaa",new hdc());
    // test get
    hdc* n = tree->get_child("aaa/bbb");
    hdc* k = n->get_child("ccc");
    // test has
    cout << tree->has_child("aaa/bbb/ccc") << endl;
    cout << tree->has_child("aaa/bbb/ddd") << endl;
    cout << tree->has_child("aaa/bbb") << endl;
    // test delete
    tree->delete_child("aaa/bbb");
    cout << tree->has_child("aaa/bbb") << endl;
    // test set
    tree->set_child("bbb/eee/aaa",new hdc());
    tree->set_child("bbb",new hdc());
    cout << tree->has_child("bbb/eee/aaa") << endl;
    
    // test data_set
    hdc* data = new hdc();
    int8_t array[4] = {1,2,3,4};
    long int shape[1];
    shape[0] = 4;
    data->set_data<int8_t>(1,shape,(void*)array);
    int8_t* aaa = data->as<int8_t*>();
    cout << (int)aaa[0] << " " << (int)aaa[2] << endl;
    
    hdc* list = new hdc();
    list->create_list(10);
    hdc* slice = list->get_slice(3);
    int8_t* bbb = slice->as<int8_t*>();
    cout << (int)bbb[0] << endl;
    delete list;
    
    
   // data->set_data(1,shape,array);
    return 0;
}
