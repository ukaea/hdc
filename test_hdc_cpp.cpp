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
    
    //test list
    hdc* list = new hdc();
    list->create_list(10);
    hdc* slice = list->get_slice(3);
    int8_t* bbb = slice->as<int8_t*>();
    cout << (int)bbb[0] << endl;
    
    // JSON serialization
    tree->add_child("data",data);
    tree->add_child("list",list);
    //tree->to_json("json.txt");
    tree->to_json("tree.txt",1);
    //delete list;
    
    // Copy
    tree->copy(0)->to_json("tree2.txt",1);
    // Resize
    hdc* resized = new hdc();
    resized->resize(tree,1);
    resized->to_json("resized.txt");
    
   // data->set_data(1,shape,array);
    
    // set string

    string str = "aaa123aaa123";
    hdc* ssss = new hdc();
    ssss->set_data(str);

    return 0;
}
