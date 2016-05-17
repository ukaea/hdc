#include "hdc.hpp"

using namespace std;

//---------------------------- Tree manipulation -----------------------------------

hdc::hdc()
{
    cout << "Creating empty node..." << endl;
    type = HDC_EMPTY;
    data = new vector<dynd::nd::array>;
    children = new unordered_map<string, hdc*>;
    //children = nullptr;
    list_elements = nullptr;
}


hdc::hdc(uint8_t i)
{
    cout << "Creating empty node..." << endl;
    type = HDC_EMPTY;
    data = new vector<dynd::nd::array>;
    children = new unordered_map<string, hdc*>; 
//    children = nullptr;
    list_elements = nullptr;
}

hdc::~hdc()
{
    cout << "Destructor called..." << endl;
    if (!children->empty()) {
        cout << "Deleting children..." << endl;
        for (auto it = children->begin(); it != children->end(); it++) delete it->second;
        children->clear();
    } else {
        delete children;
        delete data;
        if (list_elements != nullptr) {
            for (size_t i;i < list_elements->size();i++) delete &list_elements[i];
            list_elements->clear();
            delete list_elements;
        }
    }
}

bool hdc::has_child(string path)
{
    return this->has_child(split(path,'/'));
}

bool hdc::has_child(vector<string> vs)
{
    cout << "Searching for: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;

    string first = vs[0];
    vs.erase(vs.begin());

    if (this->children->count(first) == 0) return false;

    if (vs.empty()) {
        return (bool)this->children->count(first);
    } else {
        return this->children->at(first)->has_child(vs);
    }
    return false; // never goes here
}


void hdc::add_child(vector<string> vs, hdc* n) {
    cout << "Adding node: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    
    if (!(this->type == HDC_EMPTY || this->type == HDC_STRUCT)) {
        cout << "Cannot add child to this node. Data assigned???" << endl;
        return;
    }
    if (this->type == HDC_EMPTY) {
        this->type = HDC_STRUCT;
//        this->children = new unordered_map<string, hdc*>;
    }
    
    string first = vs[0];
    vs.erase(vs.begin());
    if (vs.empty()) {
        
        if (this->children->count(first) == 0) children->insert({first,n});
        else cout << "Error: child already exists!" << endl;
    } else {
        if (this->children->count(first) == 0) {
            children->insert({first,new hdc()});
        }
        children->at(first)->add_child(vs,n);
    }
        cout << "done" << endl;

    return;
}

void hdc::set_list(vector<hdc*>* list) {
    if (this->type != HDC_EMPTY) {
        cout << "Cannot add list to a non-list node." << endl;
    }
    this->list_elements = list;
    return;
}

void hdc::create_list(size_t n) {
    if (this->type != HDC_EMPTY) {
        cout << "Cannot add list to a non-list node." << endl;
    }
    this->type = HDC_LIST;
    this->list_elements = new vector<hdc*>;
    for (size_t i=0; i<n;i++) list_elements->push_back(new hdc());
    //debuging
    for (size_t i=0; i<n;i++) {
        int8_t* arr = new int8_t[1];
        arr[0] = i;
        long shape[] = {1};
        list_elements->at(i)->set_data<int8_t>(1,(long int*)shape,(void*)arr);    
    }
    return;
}

void hdc::add_child(string path, hdc* n)
{
    cout << "Adding child: " << path << endl;
    add_child(split(path,'/'),n);
    return;
}

void hdc::delete_child(vector<string> vs) {
    
    cout << "Delete node: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    
    string first = vs[0];
    vs.erase(vs.begin());
    
    if (this->children->count(first) == 0) return; // Nothing to do here
    
    if (!vs.empty()) {
        delete this->children->at(first);
        this->children->erase(first);
    } else {
        this->children->at(first)->delete_child(vs);
    }
    return;
}

void hdc::delete_child(string path) {
    this->delete_child(split(path,'/'));
    return;
}

hdc* hdc::get_child(vector<string> vs) {
    cout << "Getting node: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "::";
    cout << endl;
    
    string first = vs[0];
    vs.erase(vs.begin());
    
    if (this->children->count(first)) {
        if (vs.empty()) return this->children->at(first);
        else return this->children->at(first)->get_child(vs);
    } else {
        cout << "Not found" << endl;
        exit(-1);
        return new hdc();
    }
    
}

hdc* hdc::get_slice(vector<string> vs, size_t i) {
    cout << "Getting slice: " << i << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    
    string first = vs[0];
    vs.erase(vs.begin());
    
    if (this->children->count(first)) {
        if (vs.empty()) {
            if (i < 0 || i > this->list_elements->size()) {
                cout << "Error: index out of range!" << endl;
                return new hdc();
            }
            return this->list_elements->at(i);
        }
        else return this->children->at(first)->get_slice(vs,i);
    } else {
        cout << "Not found" << endl;
        return new hdc();
    }
}


hdc* hdc::get_slice(size_t i) {
    return this->list_elements->at(i);
}

hdc* hdc::get_slice(string path, size_t i) {
    return this->get_slice(split(path,'/'),i);
}

hdc* hdc::get_child(string path) {
    return this->get_child(split(path,'/'));
}


void hdc::set_child(vector<string> vs, hdc* n) {
    // not sure now, what should this do....
    cout << "Setting node: " << endl;
    for (size_t i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    
    if (!this->has_child(vs)) { // Nothing to set
        cout << "Nothing to set, maybe you want to add..." << endl;
        return;
    }
    
    string first = vs[0];
    vs.erase(vs.begin());

    if (vs.empty()) {
        delete this->children->at(first);
        this->children->at(first) = n;
    } else this->children->at(first)->set_child(vs, n);
    
    return;
}


void hdc::set_child(string path, hdc* n) {
    this->set_child(split(path,'/'), n);
    return;
}


//------------------ Data manipulation -------------------------------

uint8_t hdc::get_type()
{
    return this->type;
}

void hdc::set_type(uint8_t i) {
    // More to be added here later
    this->type = i;
    if (i == HDC_LIST && this->list_elements == nullptr) this->list_elements = new vector<hdc*>;
    return;
}

int8_t hdc::get_ndim()
{
    if (this->type == HDC_DYND) return this->data->at(0).get_ndim();
    else if (this->type == HDC_LIST) return 1; // Currently, we support only single container list dimension
    else return 0;
}

int8_t hdc::get_ndim(string path) {
    return this->get_child(path)->get_ndim();
}


long int* hdc::get_shape()
{
    if (this->type == HDC_DYND) {
        long int* shape = (long int*)malloc(this->data->at(0).get_ndim() * sizeof(long int));
        for (size_t i=0; i < this->data->at(0).get_ndim(); i++) shape[i] = this->data->at(0).get_shape()[i];
        //size_t* shape = &(size_t)(this->data->at(0)->get_shape()[0]);
        return shape;
    }
    else if (this->type == HDC_LIST) {
        long int* shape = (long int*)malloc(1 * sizeof(long int)); // Currently, we support only single container list dimension
        shape[0] = this->list_elements->size();
        return shape;
//         return 0;
    }
    else {
        cerr << "get_shape() method is not implemented for this type of node: " << (int)this->type << endl;
        exit(-1);
//         return 0;
    }
}

long int* hdc::get_shape(string path) {
    return  this->get_child(path)->get_shape();
}

bool hdc::is_empty()
{
    return (this->type == HDC_EMPTY);
}

//------------------ Serialization -----------------------------

// hdc* hdc::from_json(string filename)
// {
//     return new hdc(); // Change this
// }

Json::Value hdc::to_json(int mode) {
    Json::Value root;
    if (mode == 0) {
        if (this->type == HDC_DYND) {
            root = dynd::format_json(this->data->at(0)).as<string>();        
        }
        else if (this->type == HDC_STRUCT) {
            for (auto it = this->children->begin(); it != this->children->end(); it++) {
                Json::Value child;
                child[it->first] = it->second->to_json(mode);
                it->second;
                //root.append(child);
                root[it->first] = it->second->to_json(mode);
            }
        }
        else if (this->type == HDC_LIST) {
            Json::Value elements;
            for (long i=0;i<this->list_elements->size();i++) root.append(this->list_elements->at(i)->to_json(mode));        
        }
    }
    else if (mode == 1) {
            if (this->type == HDC_DYND) {
            dynd::ndt::type dt;
            switch(this->get_ndim()) { // I really don't want to spend several hours by digging into dynd sources, sorry.
                case 0:
                    dt = this->data->at(0).get_type();
                    break;
                case 1:
                    dt = this->data->at(0)(0).get_type();
                    break;
                case 2:
                    dt = this->data->at(0)(0,0).get_type();
                    break;
                case 3:
                    dt = this->data->at(0)(0,0,0).get_type();
                    break;
                case 4:
                    dt = this->data->at(0)(0,0,0,0).get_type();
                    break;
                case 5:
                    dt = this->data->at(0)(0,0,0,0,0).get_type();
                    break;
                default: // Yes, I tried more.
                    cout << "Error: unsupported number of dimensions." << endl;
                    break;
            }
            root["dtype"] = dt.str();
            root["data"] = dynd::format_json(this->data->at(0)).as<string>();        
        }
        else if (this->type == HDC_STRUCT) {
//             Json::Value children;
            for (auto it = this->children->begin(); it != this->children->end(); it++) {
                Json::Value child;
                child[it->first] = it->second->to_json(mode);
                it->second;
                root.append(child);
//                 children[it->first] = it->second->to_json(mode);
            }
//             root["dtype"] = "hdc_struct";
//             root["data"] = children;
        }
        else if (this->type == HDC_LIST) {
            Json::Value elements;
//             for (long i=0;i<this->list_elements->size();i++) elements.append(this->list_elements->at(i)->to_json(mode));
//             root["dtype"] = "hdc_list";
//             root["data"] = elements;
            for (long i=0;i<this->list_elements->size();i++) root.append(this->list_elements->at(i)->to_json(mode));

        }
//         else if (this->type == HDC_EMPTY) {
//             root["dtype"] = "hdc_empty";
//         }
    }
    return root;
}

void hdc::resize(hdc* h, int recursively)
{
    if (this->type == HDC_DYND) {
        if (!recursively) cout << "Operation is yet not supported on the DYND node." << endl;
        return;
    }
    else if (this->type == HDC_LIST) {
        if (!recursively) cout << "Operation is yet not supported on the list node." << endl;
        return;
    }
    else if (this->type == HDC_STRUCT) {
        for (auto it = h->children->begin(); it != h->children->end(); ++it) {
            if (!this->has_child(it->first)) this->add_child(it->first,it->second->copy(0));
        }
        return;
    }
    return;
}


hdc* hdc::copy(int copy_arrays)
{
    hdc* copy = new hdc();
    copy->set_type(this->get_type());
    if (this->type == HDC_STRUCT) {
        for (auto it = this->children->begin(); it != this->children->end(); ++it) {
            hdc* child = it->second->copy(copy_arrays);
            copy->add_child(it->first,it->second);
        }
    } else if (this->type == HDC_LIST) {
        for (size_t i = 0; i < this->list_elements->size(); i++)
            copy->set_slice(i,this->list_elements->at(i)->copy(copy_arrays));
    }
    else if (this->type == HDC_DYND) {
        if (copy_arrays) {
            //dynd::nd::empty(<this->data->at(0).get_type()>);
            //long size = this->data->at(0).size();
            //char *
            copy->data->push_back(dynd::nd::array(this->data->at(0)));
        } else this->set_type(HDC_EMPTY);
    }
    return copy;
}

void hdc::set_slice(size_t i, hdc* h)
{
    cout << "Setting slice " << i << endl;
    if (this->type == HDC_LIST) {
        if (i < this->list_elements->size()) this->list_elements->at(i) = h;
        else if (i == this->list_elements->size()) this->append_slice(h);
        else cout << "Error in set_slice(): Array too small." << endl;
    } else cout << "Error in set_slice(): Wrong type to call set_slice." << endl;
    return;
}

void hdc::append_slice(hdc* h) {
    this->list_elements->push_back(h);
    return;
}

void hdc::to_json(string filename, int mode)
{
    cout << "Saving output JSON to " << filename << endl;
    ofstream json_file;
    json_file.open(filename.c_str());
    // json_file << this->to_json();
    // get rid of quotes produced by writing dynd::nd::array.as<string>
    // see https://www.youtube.com/watch?v=SiUz_akTmcY for details...
    stringstream tmp;
    tmp << this->to_json(mode);
    string tmp_str = tmp.str();
    string la("["); // left after
    string lb("\"["); // left before
    replace_all(tmp_str,lb,la);
    string ra("]"); // right after
    string rb("]\""); // right before
    replace_all(tmp_str,rb,ra);
    cout << tmp_str;
    json_file << tmp_str;
    // end of quotes removal
    json_file.close();

    return;
}
/*
void hdc::set_string(string str) {
    cout << "Setting string: " << str << endl;
    dynd::nd::array arr = str;
    this->data->push_back(arr);
    return;
}*/

// ------------------- JSON stuff ----------------------------


hdc* from_json(string filename) {
    cout << "Deserializing from JSON in file " << filename << "..." << endl;
    hdc* tree = new hdc();
    Json::Value json;
    ifstream input(filename);
    input >> json;
    Json::Value* root = &json;
    // decision here based on the type of node, allways point to the current node, go throught all subnodes
    hdc* curr = tree;
    
    cout << "Object: " << root->isObject() << endl;
    cout << "null: " << root->isNull() << endl;
    cout << "String: " << root->isString() << endl;
    cout << "Numeric: " << root->isNumeric() << endl;
    cout << "Double: " << root->isDouble() << endl;
    cout << "Bool: " << root->isBool() << endl;
    cout << "Integral: " << root->isIntegral() << endl;
    cout << "Array: " << root->isArray() << endl;
    cout << "AAA: " << Json::ValueType::arrayValue << endl;
    
// // //     if (root->isArray()) {
// // //         cout << "size: " << root->size() << endl;
// // //         for (size_t i=0; i < root->size(); i++) {
// // //             
// // //             cout << root[i] << endl;
// // //         }
// // //     } else if (root->isString()) {
// // //         cout << "String: " << root;
// // //     } else if (root->)
// // //     
    
    
    return tree;
}

hdc* json_to_hdc(Json::Value* root) {
    hdc* tree = new hdc();
    switch(root->type()) {
        case Json::ValueType::nullValue:
            cout << "root is null" << endl;
            tree->set_type(HDC_EMPTY);
            break;
        case Json::ValueType::intValue:
            cout << "root is int, value = " << root->asInt() << endl;
            tree->set_data<int32_t>(root->asInt());
            break;
        case Json::ValueType::uintValue:
            cout << "root is uint, value = " << root->asUInt() << endl;
            tree->set_data<uint32_t>(root->asUInt());
            break;
        case Json::ValueType::realValue:
            cout << "root is double, value = " << root->asDouble() << endl;
            tree->set_data<double>(root->asDouble());
            break;
        case Json::ValueType::stringValue:
            cout << "root is string, value = " << root->asCString() << endl;
            tree->set_data<string>(root->asCString());
            break;
        case Json::ValueType::booleanValue:
            cout << "root is bool, value = " << root->asBool() << endl;
            tree->set_data<bool>(root->asBool());
            break;
        case Json::ValueType::arrayValue:
            cout << "root is array, size = " << root->size() << endl;
//             if (is_all_numeric(root)) {
//                 // Save as ND array
//                 // dimension = depth of tree
//                 // We should test regularity of array
//                 tree->set_type(HDC_DYND);
// //                 tree->set_json_data
//             } else {
//                 // call recursively -- save list
//                 tree->set_type(HDC_LIST);
//                 for (int i = 0;i<root->size();i++) {
//                     tree->append_slice(json_to_hdc(&(root->operator[](i))));
//                 }
//             }
            break;
        case Json::ValueType::objectValue:
            cout << "root is object, children:" << endl;
            for (Json::ValueIterator it = root->begin(); it != root->end(); it++) {
                cout << it.key() << endl;
            }
            break;
    }
    return tree;
}


int64_t detect_array_type(Json::Value* root)
{
    return 0;
}

// bool is_all_numeric(Json::Value& root)
// {
//     bool ok = true;
//     for (int i=0;i<root.size();i++) {
//         if (!root[i].isNumeric() && !root[i].isArray()) {
//             ok = false;
//             break;
//         }
//         if (root.isArray()) {
//             if (!is_all_numeric(root[i])) {
//                 ok = false;
//                 break;
//             }
//         }
//     }
//     return ok;
// }

bool is_all_numeric(Json::Value* root)
{
    bool ok = true;
    for (int i=0;i<root->size();i++) {
        if (!root->operator[](i).isNumeric() && !root->operator[](i).isArray()) {
            ok = false;
            break;
        }
        if (root->isArray()) {
            if (!is_all_numeric(&(root->operator[](i)))) {
                ok = false;
                break;
            }
        }
    }
    return ok;
}

bool is_double(Json::Value* root)
{
    if(!is_all_numeric(root)) return false;
    if (root->isDouble() && !(root->isInt64() || root->isInt())) return true;
    else if (root->isArray()) {
        for (int i=0;i<root->size();i++) {
            if (is_double(&(root->operator[](i)))) return true;
        }
    } else return false;
}

bool is_jagged(Json::Value* root)
{
    if (!root->isArray()) return false;
    int dim = 0;
    bool jagged = false;
    dim = root->operator[](0).size();
    for (int i=1; i<root->size();i++) {
        if (root->operator[](i).size() != dim) {
            return true;
            break;
        } else return is_jagged(&(root->operator[](i)));
    }
    return jagged;
}



void set_json(string json) {
    cout << "Setting JSON: " << json << endl;
    // I don't know what to do with JSON yet.
    return;
}


//------------------ String manipulation -----------------------------

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

vector<string>& split(const string &s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        if (!item.length()) {
            cerr << "Error: empty string between slashes!" << endl;
            exit(-1);
        }
        elems.push_back(item);
    }
}

void replace_all(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}


void hello__() {
    cout << "Hello from c++" << endl;
    return;
}