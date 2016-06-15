#include "hdc.hpp"
#include <dynd/json_parser.hpp>
#include <fstream>
//#define DEBUG
struct hdc_t {
    void* obj;
};


using namespace std;

//---------------------------- Tree manipulation -----------------------------------

HDC::HDC()
{
    #ifdef DEBUG
    cout << "Creating empty node..." << endl;
    #endif
    type = HDC_EMPTY;
    data = new vector<dynd::nd::array>;
    children = new unordered_map<string, HDC*>;
    //children = nullptr;
    list_elements = nullptr;
}

HDC::HDC(dynd::nd::array arr) {
    #ifdef DEBUG
    cout << "Creating DyND node..." << endl;
    #endif
    type = HDC_DYND;
    data = new vector<dynd::nd::array>;
    children = new unordered_map<string, HDC*>;
    list_elements = nullptr;
    data->push_back(arr);
}

HDC::~HDC()
{
    #ifdef DEBUG
    cout << "Destructor called..." << endl;
    #endif
    if (!children->empty()) {
        #ifdef DEBUG
        cout << "Deleting children..." << endl;
        #endif
        for (auto it = children->begin(); it != children->end(); it++) {
            delete it->second;
            cout << "Remove: " << it->first << endl;
        }
        children->clear();
    } else {
        delete children;
        delete data;
        if (list_elements != nullptr) {
            for (size_t i = 0;i < list_elements->size();i++) delete &list_elements[i];
            list_elements->clear();
            delete list_elements;
        }
    }
}

bool HDC::has_child(string path)
{
    return this->has_child(split(path,'/'));
}

bool HDC::has_child(vector<string> vs)
{
    #ifdef DEBUG
    cout << "Searching for: " << endl;
    for (long i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    #endif
    if(vs.empty()) return false; //TODO: re-do this!!!
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


void HDC::add_child(vector<string> vs, HDC* n) {
    #ifdef DEBUG
    cout << "Adding node: " << endl;
    for (long i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    #endif
    if (!(this->type == HDC_EMPTY || this->type == HDC_STRUCT)) {
        cout << "Cannot add child to this node. Data assigned???" << endl;
        return;
    }
    if (this->type == HDC_EMPTY) {
        this->type = HDC_STRUCT;
//        this->children = new unordered_map<string, HDC*>;
    }

    string first = vs[0];
    vs.erase(vs.begin());
    if (vs.empty()) {
        if (this->children->count(first) == 0) children->insert({first,n});
        else cout << "Error: child already exists!" << endl;
    } else {
        if (this->children->count(first) == 0) {
            children->insert({first,new HDC()});
        }
        children->at(first)->add_child(vs,n);
    }
    #ifdef DEBUG
    cout << "done" << endl;
    #endif
    return;
}

void HDC::set_list(deque<HDC*>* lst) {
    if (this->type != HDC_EMPTY) {
        cout << "Cannot add list to a non-list node." << endl;
    }
    this->list_elements = lst;
    return;
}

void HDC::add_child(string path, HDC* n)
{
    #ifdef DEBUG
    cout << "Adding node: " << path << endl;
    #endif
    
    add_child(split(path,'/'),n);
    return;
}

void HDC::delete_child(vector<string> vs) {

    #ifdef DEBUG
    cout << "Delete node: " << endl;
    for (long i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    #endif
    if (!this->has_child(vs) || vs.empty())  {
        return;
    }
    string first = vs[0];
    vs.erase(vs.begin());
    if (vs.empty()) {
        this->children->erase(first);
    } else {
        this->children->at(first)->delete_child(vs);
    }
    // set type back to empty if the only child was deleted.
    if (children->empty()) this->set_type(HDC_EMPTY);
    return;
}

void HDC::delete_child(string path) {
    this->delete_child(split(path,'/'));
    return;
}

HDC* HDC::get(vector<string> vs) {
    #ifdef DEBUG
    cout << "Getting node: " << endl;
    for (long i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    #endif
    string first = vs[0];
    vs.erase(vs.begin());

    if (this->children->count(first)) {
        if (vs.empty()) return this->children->at(first);
        else return this->children->at(first)->get(vs);
    } else {
        cout << "Not found" << endl;
        return new HDC();
    }
}

HDC* HDC::get_slice(vector<string> vs, size_t i) {
    #ifdef DEBUG
    cout << "Getting slice: " << i << endl;
    for (long i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    #endif
    string first = vs[0];
    vs.erase(vs.begin());
    if (this->children->count(first)) {
        if (vs.empty()) {
            if (this->type != HDC_LIST) return this;
            if (i < 0 || i > this->list_elements->size()) {
                cout << "Error: index out of range!" << endl;
                return new HDC();
            }
            return this->list_elements->at(i);
        }
        else return this->children->at(first)->get_slice(vs,i);
    } else {
        cout << "Not found" << endl;
        return new HDC();
    }
}


HDC* HDC::get_slice(size_t i) {
    if (this->type == HDC_LIST) return this->list_elements->at(i);
    else return this; // return this if not list
}

HDC* HDC::get_slice(string path, size_t i) {
    return this->get_slice(split(path,'/'),i);
}

HDC* HDC::get(string path) {
    return this->get(split(path,'/'));
}


void HDC::set_child(vector<string> vs, HDC* n) {
    #ifdef DEBUG
    cout << "Setting node: " << endl;
    for (long i = 0; i < vs.size(); i++) cout << vs[i] << "/";
    cout << endl;
    #endif
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


void HDC::set_child(string path, HDC* n) {
    this->set_child(split(path,'/'), n);
    return;
}

//------------------ Data manipulation -------------------------------

uint8_t HDC::get_type()
{
    return this->type;
}

void HDC::set_type(uint8_t i) {
    // More to be added here later
    this->type = i;
    if (i == HDC_LIST && this->list_elements == nullptr) this->list_elements = new deque<HDC*>;
    return;
}

int8_t HDC::get_ndim()
{
    if (this->type == HDC_DYND) return this->data->at(0).get_ndim();
    else if (this->type == HDC_LIST) return 1; // Currently, we support only single container list dimension
    else return 1;
}

int8_t HDC::get_ndim(string path) {
    return this->get(path)->get_ndim();
}

void* HDC::as_void_ptr() {
    return (void*)this;
}


long int* HDC::get_shape()
{
    if (this->type == HDC_DYND) {
        long int* shape = (long int*)malloc(this->data->at(0).get_ndim() * sizeof(long int));
        for (long i=0; i < this->data->at(0).get_ndim(); i++) shape[i] = this->data->at(0).get_shape()[i];
        return shape;
    }
    else if (this->type == HDC_LIST) {
        long int* shape = (long int*)malloc(1 * sizeof(long int)); // Currently, we support only single container list dimension
        shape[0] = this->list_elements->size();
        return shape;
    }
    else if (this->type == HDC_EMPTY) {
        long int* shape = (long int*)malloc(1 * sizeof(long int));
        // empty shape = (0, )
        shape[0] = 0;
        return shape;
    }
    else if (this->type == HDC_STRUCT) {
        long int* shape = (long int*)malloc(1 * sizeof(long int)); 
        // container type has (1, ) shape
        shape[0] = 1;
        return shape;
    }
    else {
        // error, should not get here
        cout << "Unknown type" << endl;
        long int* shape = (long int*)malloc(1 * sizeof(long int)); 
        shape[0] = -1;
        return shape;
    }
}

long int* HDC::get_shape(string path) {
    if (!this->has_child(path)) {
        cerr << "Not found (get_shape): " << path << endl;
        exit(-1);
    }
    return this->get(path)->get_shape();
}

bool HDC::is_empty()
{
    return (this->type == HDC_EMPTY);
}

//------------------ Serialization -----------------------------

HDC* from_json(const string& filename)
{
    HDC* tree;
    ifstream file;
    file.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        file.open(filename);
        Json::Value root;
        file >> root;
        tree = json_to_hdc(root);
    }
    catch (ifstream::failure e) {
        cout << "Error reading / opening file." << endl;
    }
    file.close();
    return tree;
}

Json::Value HDC::to_json(int mode) {
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
            for (size_t i=0;i<this->list_elements->size();i++) root.append(this->list_elements->at(i)->to_json(mode));
        }
    }
    else if (mode == 1) {
            if (this->type == HDC_DYND) {
            dynd::ndt::type dt;
            switch(this->get_ndim()) {
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
                default:
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
            for (size_t i=0;i<this->list_elements->size();i++) root.append(this->list_elements->at(i)->to_json(mode));

        }
//         else if (this->type == HDC_EMPTY) {
//             root["dtype"] = "hdc_empty";
//         }
    }
    return root;
}

void HDC::dump() {
    
    // get rid of quotes produced by writing dynd::nd::array.as<string>
    stringstream tmp;
    tmp << this->to_json(0);
    string tmp_str = tmp.str();
    string la("["); // left after
    string lb("\"["); // left before
    replace_all(tmp_str,lb,la);
    string ra("]"); // right after
    string rb("]\""); // right before
    replace_all(tmp_str,rb,ra);
    string sa(""); // slash after
    string sb("\\\""); // slash before
    replace_all(tmp_str,sb,sa);
    cout << tmp_str;
    return;
}


void HDC::resize(HDC* h, int recursively)
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

void HDC::set_dynd(dynd::nd::array array) {
    if (this->type != HDC_EMPTY) return;
    this->data->push_back(array);
    return;
}

HDC* HDC::copy(int copy_arrays)
{
    #ifdef DEBUG
    cout << "Called copy()" << endl;
    #endif
    HDC* copy = new HDC();
    copy->set_type(this->get_type());
    #ifdef DEBUG
    cout << (int)(this->get_type()) << endl;
    #endif
    if (this->type == HDC_STRUCT) {
        for (auto it = this->children->begin(); it != this->children->end(); it++) {
            copy->add_child(it->first,it->second->copy(copy_arrays));
            #ifdef DEBUG
            cout << it->first << " copied" << endl;
            #endif
        }
    } else if (this->type == HDC_LIST) {
        for (size_t i = 0; i < this->list_elements->size(); i++)
            copy->insert_slice(i,this->list_elements->at(i)->copy(copy_arrays));
    }
    else if (this->type == HDC_DYND) {
        if (copy_arrays == 1) {
            //dynd::nd::empty(<this->data->at(0).get_type()>);
            //long size = this->data->at(0).size();
            //char *
            copy->data->push_back(dynd::nd::array(this->data->at(0)));
            #ifdef DEBUG
            cout << "copy: " << copy->data->at(0) << endl;
            #endif
        } else this->set_type(HDC_EMPTY);
    }
    #ifdef DEBUG
    cout << "copy() Done" << endl;
    #endif
    return copy;
}

void HDC::insert_slice(size_t i, HDC* h)
{
    #ifdef DEBUG
    cout << "Setting slice " << i << endl;
    #endif
    switch (this->type) {
        case HDC_EMPTY:
            this->set_type(HDC_LIST);
        case HDC_LIST:
            this->list_elements->emplace(this->list_elements->begin()+i,h);
            break;
        //TODO: Add support for HDC_STRUCT here? How?
        default:
            cout << "Error in insert_slice(): Wrong type to call insert_slice." << endl;
            break;
    }
    return;
}

void HDC::set_slice(size_t i, HDC* h)
{
    #ifdef DEBUG
    cout << "Setting slice " << i << endl;
    #endif
    switch (this->type) {
        case HDC_LIST:
            if (i >= this->list_elements->size()) {
                cout << "Error in set_slice: Index " << i << " >= list size." << endl;
                return;
            }
            this->list_elements->at(i) = h;
            break;
            //TODO: Add support for HDC_STRUCT here? How?
        default:
            cout << "Error in insert_slice(): Wrong type to call insert_slice." << endl;
            break;
    }
    return;
}


void HDC::append_slice(HDC* h) {
    if (this->type == HDC_EMPTY) {
        this->set_type(HDC_LIST);
    }
    if (this->type == HDC_LIST) {
        this->list_elements->push_back(h);
    } else {
        cout << "Error: append_slice not supported for HDC_STRUCT" << endl;
        exit(-1);
    }
    return;
}

hdc_t* HDC::as_hdc_ptr() {
    hdc_t* wrap = new struct hdc_t;
    wrap->obj = (void*) this;
    return wrap;
}

string HDC::get_type_str() {
    string type_str;
    if (this->type == HDC_EMPTY) type_str = "null";
    else if (this->type == HDC_LIST || this->type == HDC_STRUCT) type_str = "hdc";
    else {
        dynd::ndt::type dt;
        switch(this->get_ndim()) {
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
            default:
                cout << "Error: unsupported number of dimensions." << endl;
                break;
        }
        type_str = dt.str();
    }
    return type_str;
}

string HDC::get_type_str(string path) {
    return this->get(path)->get_type_str();
}

string HDC::get_datashape_str(string path) {
    return this->get(path)->get_datashape_str();
}

string HDC::get_datashape_str() {
    string type_str;
    if (this->type == HDC_EMPTY) type_str = "null";
    else if (this->type == HDC_LIST || this->type == HDC_STRUCT) type_str = "hdc";
    else {
        dynd::ndt::type dt;
        dt = this->data->at(0).get_type();
        type_str = dt.str();
    }
    return type_str;
}


void HDC::to_json(string filename, int mode)
{
    #ifdef DEBUG
    cout << "Saving output JSON to " << filename << endl;
    #endif
    ofstream json_file;
    json_file.open(filename.c_str());
    // json_file << this->to_json();
    // get rid of quotes produced by writing dynd::nd::array.as<string>
    stringstream tmp;
    tmp << this->to_json(mode);
    string tmp_str = tmp.str();
    string la("["); // left after
    string lb("\"["); // left before
    replace_all(tmp_str,lb,la);
    string ra("]"); // right after
    string rb("]\""); // right before
    replace_all(tmp_str,rb,ra);
    string sa(""); // slash after
    string sb("\\\""); // slash before
    replace_all(tmp_str,sb,sa);
    #ifdef DEBUG
    cout << tmp_str;
    #endif
    json_file << tmp_str;
    // end of quotes removal
    json_file.close();

    return;
}

HDC* json_to_hdc(Json::Value* root) {
    HDC* tree = new HDC();
    switch(root->type()) {
        case Json::ValueType::nullValue:
            #ifdef DEBUG
            cout << "root is null" << endl;
            #endif
            tree->set_type(HDC_EMPTY);
            break;
        case Json::ValueType::intValue:
            #ifdef DEBUG
            cout << "root is int, value = " << root->asInt() << endl;
            #endif
            tree->set_data<int32_t>(root->asInt());
            break;
        case Json::ValueType::uintValue:
            #ifdef DEBUG
            cout << "root is uint, value = " << root->asUInt() << endl;
            #endif
            tree->set_data<uint32_t>(root->asUInt());
            break;
        case Json::ValueType::realValue:
            #ifdef DEBUG
            cout << "root is double, value = " << root->asDouble() << endl;
            #endif
            tree->set_data<double>(root->asDouble());
            break;
        case Json::ValueType::stringValue:
            #ifdef DEBUG
            cout << "root is string, value = " << root->asCString() << endl;
            #endif
            tree->set_data<string>(root->asCString());
            break;
        case Json::ValueType::booleanValue:
            #ifdef DEBUG
            cout << "root is bool, value = " << root->asBool() << endl;
            #endif
            tree->set_data<bool>(root->asBool());
            break;
        case Json::ValueType::arrayValue:
            #ifdef DEBUG
            cout << "root is array, size = " << root->size() << endl;
            #endif
            if (is_all_numeric(root)) {
                int8_t ndim = get_ndim(root);
                long* shape = get_shape(root);
                stringstream ss;
                ss << *root;
                dynd::ndt::type dt;
                if (is_double(root)) dt = dynd::ndt::make_type<double>();
                else dt = dynd::ndt::make_type<int32_t>();
                dynd::nd::array array = dynd::nd::dtyped_ones(ndim,shape,dt);

                switch(ndim) {
                    case 1:
                        if (is_double(root)) {
                            for (int i = 0; i < shape[0]; i++) array(i).assign(root->operator[](i).asDouble());
                        } else {
                            for (int i = 0; i < shape[0]; i++) array(i).assign(root->operator[](i).asInt());
                        }
                        break;
                    case 2:
                        if (is_double(root)) {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    array(i,j).assign(root->operator[](i).operator[](j).asDouble());
                        } else {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    array(i,j).assign(root->operator[](i).operator[](j).asInt());
                        }
                        break;
                    case 3:
                        if (is_double(root)) {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        array(i,j,k).assign(root->operator[](i).operator[](j).operator[](k).asDouble());
                        } else {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        array(i,j,k).assign(root->operator[](i).operator[](j).operator[](k).asInt());
                        }
                        break;
                    case 4:
                        if (is_double(root)) {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        for (int l = 0; l < shape[3]; l++)
                                            array(i,j,k,l).assign(root->operator[](i).operator[](j).operator[](k).operator[](l).asDouble());
                        } else {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        for (int l = 0; l < shape[3]; l++)
                                            array(i,j,k,l).assign(root->operator[](i).operator[](j).operator[](k).operator[](l).asInt());
                        }
                        break;
                    case 5:
                        if (is_double(root)) {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        for (int l = 0; l < shape[3]; l++)
                                            for (int m = 0; m < shape[3]; m++)
                                                array(i,j,k,l,m).assign(root->operator[](i).operator[](j).operator[](k).operator[](l).operator[](m).asDouble());
                        } else {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        for (int l = 0; l < shape[3]; l++)
                                            for (int m = 0; m < shape[3]; m++)
                                                array(i,j,k,l,m).assign(root->operator[](i).operator[](j).operator[](k).operator[](l).operator[](m).asInt());
                        }
                        break;
                }
                tree->set_dynd(array);
                tree->set_type(HDC_DYND);
            } else {
                // call recursively -- save list
                tree->set_type(HDC_LIST);
                for (unsigned int i = 0;i<root->size();i++) {
                    tree->append_slice(json_to_hdc(&(root->operator[](i))));
                }
            }
            break;
        case Json::ValueType::objectValue:
            #ifdef DEBUG
            cout << "root is object, children:" << endl;
            #endif
            for (Json::ValueIterator it = root->begin(); it != root->end(); it++) {
                #ifdef DEBUG
                cout << it.key() << endl;
                #endif
                tree->add_child(it.key().asCString(),json_to_hdc(*it));
            }
            break;
    }
    return tree;
}


HDC* json_to_hdc(const Json::Value& root) {
    HDC* tree = new HDC();
    switch(root.type()) {
        case Json::ValueType::nullValue:
            #ifdef DEBUG
            cout << "root is null" << endl;
            #endif
            tree->set_type(HDC_EMPTY);
            break;
        case Json::ValueType::intValue:
            #ifdef DEBUG
            cout << "root is int, value = " << root.asInt() << endl;
            #endif
            tree->set_data<int32_t>(root.asInt());
            break;
        case Json::ValueType::uintValue:
            #ifdef DEBUG
            cout << "root is uint, value = " << root.asUInt() << endl;
            #endif
            tree->set_data<uint32_t>(root.asUInt());
            break;
        case Json::ValueType::realValue:
            #ifdef DEBUG
            cout << "root is double, value = " << root.asDouble() << endl;
            #endif
            tree->set_data<double>(root.asDouble());
            break;
        case Json::ValueType::stringValue:
            #ifdef DEBUG
            cout << "root is string, value = " << root.asCString() << endl;
            #endif
            tree->set_data<string>(root.asCString());
            break;
        case Json::ValueType::booleanValue:
            #ifdef DEBUG
            cout << "root is bool, value = " << root.asBool() << endl;
            #endif
            tree->set_data<bool>(root.asBool());
            break;
        case Json::ValueType::arrayValue:
            #ifdef DEBUG
            cout << "root is array, size = " << root.size() << endl;
            #endif
            if (is_all_numeric(root)) {
                int8_t ndim = get_ndim(root);
                long* shape = get_shape(root);
                stringstream ss;
                ss << root;
                dynd::ndt::type dt;
                if (is_double(root)) dt = dynd::ndt::make_type<double>();
                else dt = dynd::ndt::make_type<int32_t>();
                dynd::nd::array array = dynd::nd::dtyped_ones(ndim,shape,dt);

                switch(ndim) {
                    case 1:
                        if (is_double(root)) {
                            for (int i = 0; i < shape[0]; i++) array(i).assign(root[i].asDouble());
                        } else {
                            for (int i = 0; i < shape[0]; i++) array(i).assign(root[i].asInt());
                        }
                        break;
                    case 2:
                        if (is_double(root)) {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    array(i,j).assign(root[i][j].asDouble());
                        } else {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    array(i,j).assign(root[i][j].asInt());
                        }
                        break;
                    case 3:
                        if (is_double(root)) {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        array(i,j,k).assign(root[i][j][k].asDouble());
                        } else {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        array(i,j,k).assign(root[i][j][k].asInt());
                        }
                        break;
                    case 4:
                        if (is_double(root)) {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        for (int l = 0; l < shape[3]; l++)
                                            array(i,j,k,l).assign(root[i][j][k][l].asDouble());
                        } else {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        for (int l = 0; l < shape[3]; l++)
                                            array(i,j,k,l).assign(root[i][j][k][l].asInt());
                        }
                        break;
                    case 5:
                        if (is_double(root)) {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        for (int l = 0; l < shape[3]; l++)
                                            for (int m = 0; m < shape[3]; m++)
                                                array(i,j,k,l,m).assign(root[i][j][k][l][m].asDouble());
                        } else {
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    for (int k = 0; k < shape[2]; k++)
                                        for (int l = 0; l < shape[3]; l++)
                                            for (int m = 0; m < shape[3]; m++)
                                                array(i,j,k,l,m).assign(root[i][j][k][l][m].asInt());
                        }
                        break;
                }
                tree->set_dynd(array);
                tree->set_type(HDC_DYND);
            } else {
                // call recursively -- save list
                tree->set_type(HDC_LIST);
                for (unsigned int i = 0;i<root.size();i++) {
                    tree->append_slice(json_to_hdc((root[i])));
                }
            }
            break;
                    case Json::ValueType::objectValue:
                        #ifdef DEBUG
                        cout << "root is object, children:" << endl;
                        #endif
                        for (Json::ValueConstIterator it = root.begin(); it != root.end(); it++) {
                            #ifdef DEBUG
                            cout << it.key() << endl;
                            #endif
                            tree->add_child(it.key().asCString(),json_to_hdc(*it));
                        }
                        break;
    }
    return tree;
}

bool is_all_numeric(Json::Value* root)
{
    bool ok = true;
    for (unsigned int i=0;i<root->size();i++) {
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

bool is_all_numeric(const Json::Value& root)
{
    bool ok = true;
    for (unsigned int i=0;i<root.size();i++) {
        if (!root[i].isNumeric() && !root[i].isArray()) {
            ok = false;
            break;
        }
        if (root.isArray()) {
            if (!is_all_numeric(&(root[i]))) {
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
        for (unsigned int i=0;i<root->size();i++) {
            if (is_double(&(root->operator[](i)))) return true;
        }
    } else return false;
    return false;
}

bool is_double(const Json::Value& root)
{
    if(!is_all_numeric(root)) return false;
    if (root.isDouble() && !(root.isInt64() || root.isInt())) return true;
    else if (root.isArray()) {
        for (unsigned int i=0;i<root.size();i++) {
            if (is_double(root[i])) return true;
        }
    } else return false;
    return false;
}


bool is_jagged(Json::Value* root)
{
    if (!root->isArray()) return false;
    unsigned int dim = 0;
    bool jagged = false;
    dim = root->operator[](0).size();
    for (unsigned int i=0; i<root->size();i++) {
        if (root->operator[](i).size() != dim) {
            return true;
            break;
        } else return is_jagged(&(root->operator[](i)));
    }
    return jagged;
}

bool is_jagged(const Json::Value& root)
{
    if (!root.isArray()) return false;
    unsigned int dim = 0;
    bool jagged = false;
    dim = root[0].size();
    for (unsigned int i=0; i<root.size();i++) {
        if (root[i].size() != dim) {
            return true;
            break;
        } else return is_jagged(root[i]);
    }
    return jagged;
}


long* get_shape(Json::Value* root) {
    if (!root->isArray()) return 0;
    int dim = 0;
    std::vector<long> shape;
    Json::Value* curr = root;
    while (curr->isArray()) {
        shape.push_back(curr->size());
        curr = &(curr->operator[](0));
        dim++;
    }
    #ifdef DEBUG
    cout << "Dimension: " << dim << endl;
    cout << "Shape: (" << shape[0];
    for (int i=1; i<dim; i++) cout << ", " << shape[i];
    cout << ")" << endl;
    #endif
    long* res = new long[dim];
    for (int i=0;i<dim;i++) res[i] = shape[i];
    return res;
}

long* get_shape(const Json::Value& root) {
    if (!root.isArray()) return 0;
    int dim = 0;
    std::vector<long> shape;
    Json::Value curr = root;
    while (curr.isArray()) {
        shape.push_back(curr.size());
        curr = curr[0];
        dim++;
    }
    #ifdef DEBUG
    cout << "Dimension: " << dim << endl;
    cout << "Shape: (" << shape[0];
    for (int i=1; i<dim; i++) cout << ", " << shape[i];
    cout << ")" << endl;
    #endif
    long* res = new long[dim];
    for (int i=0;i<dim;i++) res[i] = shape[i];
    return res;
}

int8_t get_ndim(Json::Value* root) {
    if (!root->isArray()) return 0;
    int dim = 0;
    Json::Value* curr = root;
    while (curr->isArray()) {
        curr = &(curr->operator[](0));
        dim++;
    }
    return dim;
}

int8_t get_ndim(const Json::Value& root) {
    if (!root.isArray()) return 0;
    int dim = 0;
    Json::Value curr = root;
    while (curr.isArray()) {
        curr = curr[0];
        dim++;
    }
    return dim;
}



//------------------ String manipulation -----------------------------

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void split(const string &s, char delim, vector<string>& elems) {
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
