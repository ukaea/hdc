#include <iostream>
#include "hdc.hpp"

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
    
    // test data rewrite
    int8_t array2[4] = {9,8,7,6};
    data->set_data<int8_t>(1,shape,(void*)array2);
    int8_t* aaa2 = data->as<int8_t*>();
    cout << (int)aaa2[0] << " " << (int)aaa2[2] << endl;
    getchar();
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
    tree->copy(1)->to_json("tree2.txt",1);
    
    
    // Resize
    hdc* resized = new hdc();
    resized->resize(tree,1);
    resized->to_json("resized.txt");
    
//    // data->set_data(1,shape,array);
//     
//     // set string
//     string str = "aaa123aaa123";
//     hdc* ssss = new hdc();
//     ssss->set_data(str);
//     
//     // Test deserialization here
//     //hdc* fj = from_json("tree2.txt");
//     hdc* fi = json_to_hdc(new Json::Value(5));
//     hdc* fu = json_to_hdc(new Json::Value(5u));
//     hdc* fd = json_to_hdc(new Json::Value(5.));
//     hdc* fb = json_to_hdc(new Json::Value(true));
//     hdc* fn = json_to_hdc(new Json::Value(Json::nullValue));
//     hdc* fs = json_to_hdc(new Json::Value("aaaa"));
//     Json::Value* ar = new Json::Value();
//     ar->append(new Json::Value(8));
//     ar->append(new Json::Value(6));
//     hdc* fa = json_to_hdc(ar);
//     Json::Value* obj = new Json::Value();
//     obj->operator[]("aaa") = new Json::Value("aaaaaa");
//     obj->operator[]("bbb") = new Json::Value("bbbbbb");
//     hdc* fo = json_to_hdc(obj);
//     
    
    // test is_jagged()
//     Json::Value* a2 = new Json::Value();
//     a2->append(ar);
//     a2->append(ar);
//     a2[1].append(new Json::Value(55));
//     cout << is_jagged(a2) << endl;
//     // test is_all_numeric()
//     Json::Value aa;
//     aa.append(Json::Value(5));
//     aa.append(Json::Value(-3));
//     aa.append(Json::Value(5u));
//     aa.append(Json::Value(5.));
//     cout << "is_all_numeric: " << is_all_numeric(&aa) << endl;
//     aa.append(Json::Value("aa"));
//     cout << "is_all_numeric: " << is_all_numeric(&aa) << endl;
//     //
// //     Json::Value bb;
// //     bb.append(Json::Value(5.));
// //     bb.append(Json::Value(5.));
// //     Json::Value a2;
// //     a2.append(bb);
// //     a2.append(bb);
// //     cout << "is_all_numeric: " << is_all_numeric(&a2) << endl;
// //     Json::Value a3;
// //     a3.append(a2);
// //     a3.append(a2);
// //     cout << a3 << endl;
// //     cout << "is_all_numeric: " << is_all_numeric(&a3) << endl;
// //     a3[0][0][0] = Json::Value("aa");
// //     cout << a3 << endl;
// //     cout << "is_all_numeric: " << is_all_numeric(&a3) << endl;
// //     // test is_double()
// //     cout << "is_double: " << is_double(new Json::Value(5.1)) << endl;
// //     cout << "is_double: " << is_double(new Json::Value(5)) << endl;
// //     cout << "is_double: " << is_double(new Json::Value("aaa")) << endl;
// //     cout << "is_double: " << is_double(&a3) << endl;
// //     a3[0][0][0] = Json::Value(5.5);
// //     cout << "is_double: " << is_double(&a3) << endl;
// //     a3[0][0][0] = Json::Value(5);
// //     cout << "is_double: " << is_double(&a3) << endl;
// //     //test array deserialization:
// //     
// //     
    
    
    
    cout << "--- Testing get_type_str" << endl;
    hdc* t = new hdc();
    cout << "type: " << t->get_type_str() << endl;
    t->add_child("a",new hdc());
    cout << "type: " << t->get_type_str() << endl;
    hdc *tt = new hdc();
    tt->set_data<int8_t>(1,shape,(void*)array);
    cout << "type: " << tt->get_type_str() << endl;
    cout << "type: " << tt->get_datashape_str() << endl;
    return 0;
}
