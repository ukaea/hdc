#include "hdc.hpp"
#include <json/json.h>



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

size_t* get_shape(const Json::Value& root) {
    if (!root.isArray()) return 0;
    int dim = 0;
    size_t shape[HDC_MAX_DIMS];
    Json::Value curr = root;
    while (curr.isArray()) {
        shape[dim] = curr.size();
        curr = curr[0];
        dim++;
    }
    #ifdef DEBUG
    cout << "Dimension: " << dim << endl;
    cout << "Shape: (" << shape[0];
    for (int i=1; i<dim; i++) cout << ", " << shape[i];
    cout << ")" << endl;
    #endif
    size_t* res = new size_t[dim];
    for (int i=0;i<dim;i++) res[i] = shape[i];
    return res;
}



int get_ndim(const Json::Value& root) {
    if (!root.isArray()) return 0;
    int dim = 0;
    Json::Value curr = root;
    while (curr.isArray()) {
        curr = curr[0];
        dim++;
    }
    return dim;
}

HDC* json_to_hdc(const Json::Value& root) {
    HDC* tree = new HDC();
    switch(root.type()) {
        {
        case(Json::nullValue):
            #ifdef DEBUG
            cout << "root is null" << endl;
            #endif
            tree->set_type(EMPTY_ID);
            break;
        }
        case(Json::intValue):
        {
            #ifdef DEBUG
            cout << "root is int, value = " << root.asInt() << endl;
            #endif
            tree->set_data<int32_t>(root.asInt());
            break;
        }
        case(Json::uintValue):
        {
            #ifdef DEBUG
            cout << "root is uint, value = " << root.asUInt() << endl;
            #endif
            tree->set_data<uint32_t>(root.asUInt());
            break;
        }
        case(Json::realValue):
        {
            #ifdef DEBUG
            cout << "root is double, value = " << root.asDouble() << endl;
            #endif
            tree->set_data(root.asDouble());
            break;
        }
        case(Json::stringValue):
        {
            #ifdef DEBUG
            cout << "root is string, value = " << root.asCString() << endl;
            #endif
            tree->set_string(root.asCString());
            break;
        }
        case(Json::booleanValue):
        {
            #ifdef DEBUG
            cout << "root is bool, value = " << root.asBool() << endl;
            #endif
            tree->set_data<bool>(root.asBool());
            break;
        }
        case(Json::arrayValue):
        {
            #ifdef DEBUG
            cout << "root is array, size = " << root.size() << endl;
            #endif
            if (is_all_numeric(root)) {
                int8_t ndim = get_ndim(root);
                if (ndim > HDC_MAX_DIMS) {
                    cerr << "Unsupported number of dimensions: " << ndim << endl;
                    exit(-5);
                }
                size_t* shape = get_shape(root);
                TypeID dt;
                if (is_double(root)) dt = DOUBLE_ID;
                else dt = INT32_ID;
                delete tree;
                tree = new HDC(ndim,shape,dt);
                void* data_ptr = tree->as<void*>();
                if (dt == DOUBLE_ID) {
                    switch(ndim) {
                        case 1:
                        {
                            andres::View<double> view(shape, shape+1, (double*)data_ptr);
                            for (int i = 0; i < shape[0]; i++) {
                                view(i) = root[i].asDouble();
                            }
                            break;
                        }
                        case 2:
                        {
                            andres::View<double> view(shape, shape+2, (double*)data_ptr);
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    view(i,j) = root[i][j].asDouble();
                            break;
                        }
                        default:
                        {
                            cerr << "Unsupported number of dimensions: " << ndim << endl;
                            exit(-5);
                        }
                    }
                }
                else {
                    switch(ndim) {
                        case 1:
                        {
                            andres::View<int32_t> view(shape, shape+1, (int32_t*)data_ptr);
                            for (int i = 0; i < shape[0]; i++) view(i) = root[i].asInt();
                            break;
                        }
                        case 2:
                        {
                            andres::View<int32_t> view(shape, shape+2, (int32_t*)data_ptr);
                            for (int i = 0; i < shape[0]; i++)
                                for (int j = 0; j < shape[1]; j++)
                                    view(i,j) = root[i][j].asInt();
                            break;
                        }
                        default:
                        {
                            cerr << "Unsupported number of dimensions: " << ndim << endl;
                            exit(-5);
                        }
                    }
                }
                delete[] shape;
            } else {
                // call recursively -- save list
                tree->set_type(LIST_ID);
                for (unsigned int i = 0;i<root.size();i++) {
                    tree->append_slice(json_to_hdc(root[i]));
                }
            }
            break;
        }
        case(Json::objectValue):
        {
            #ifdef DEBUG
            cout << "root is object, children:" << endl;
            #endif
            for (Json::ValueConstIterator it = root.begin(); it != root.end(); it++) {
                #ifdef DEBUG
                cout << "KEY: " << it.key() << endl;
                #endif
                tree->add_child(it.key().asCString(),json_to_hdc(*it));
            }
            break;
        }
    }
    return tree;
}

template<typename T>
Json::Value buffer_to_json(char* buffer, int ndim, size_t* shape) {
    // TODO: Add Fortran column order
    
    Json::Value root;
    andres::View<T> view(shape, shape+ndim, (T*)buffer);
    switch(ndim) {
        case(0):
        {
            root = ((T*)buffer)[0];
            break;
        }
        case (1):
        {
            for (int i0=0;i0<shape[0];i0++)
                root[i0] = (T)view(i0);
            break;
        }
        case (2):
        {
            for (int i0=0;i0<shape[0];i0++)
                for (int i1=0;i1<shape[1];i1++)
                    root[i0][i1] = view(i0,i1);
            break;
        }
        case (3):
        {
            for (int i0=0;i0<shape[0];i0++)
                for (int i1=0;i1<shape[1];i1++)
                    for (int i2=0;i2<shape[2];i2++)
                        root[i0][i1][i2] = view(i0,i1,i2);
            break;
        }
        case (4):
        {
            for (int i0=0;i0<shape[0];i0++)
                for (int i1=0;i1<shape[1];i1++)
                    for (int i2=0;i2<shape[2];i2++)
                        for (int i3=0;i3<shape[3];i3++)
                            root[i0][i1][i2][i3] = view(i0,i1,i2,i3);
            break;
        }
        case (5):
        {
            for (int i0=0;i0<shape[0];i0++)
                for (int i1=0;i1<shape[1];i1++)
                    for (int i2=0;i2<shape[2];i2++)
                        for (int i3=0;i3<shape[3];i3++)
                            for (int i4=0;i4<shape[4];i4++)
                                root[i0][i1][i2][i3][i4] = view(i0,i1,i2,i3,i4);
            break;
        }
        /*
        case (6):
        {
            for (int i0=0;i0<shape[0];i0++)
                for (int i1=0;i1<shape[1];i1++)
                    for (int i2=0;i2<shape[2];i2++)
                        for (int i3=0;i3<shape[3];i3++)
                            for (int i4=0;i4<shape[4];i4++)
                                for (int i5=0;i5<shape[5];i5++)
                                    root[i0][i1][i2][i3][i4][i5] = view(i0,i1,i2,i3,i4,i5);
            break;
        }
        case (7):
        {
            for (int i0=0;i0<shape[0];i0++)
                for (int i1=0;i1<shape[1];i1++)
                    for (int i2=0;i2<shape[2];i2++)
                        for (int i3=0;i3<shape[3];i3++)
                            for (int i4=0;i4<shape[4];i4++)
                                for (int i5=0;i5<shape[5];i5++)
                                    for (int i6=0;i6<shape[5];i6++)
                                        root[i0][i1][i2][i3][i4][i5][i6] = view(i0,i1,i2,i3,i4,i5,i6);
            break;
        }
        case (8):
        {
            for (int i0=0;i0<shape[0];i0++)
                for (int i1=0;i1<shape[1];i1++)
                    for (int i2=0;i2<shape[2];i2++)
                        for (int i3=0;i3<shape[3];i3++)
                            for (int i4=0;i4<shape[4];i4++)
                                for (int i5=0;i5<shape[5];i5++)
                                    for (int i6=0;i6<shape[5];i6++)
                                        for (int i7=0;i7<shape[5];i7++)
                                            root[i0][i1][i2][i3][i4][i5][i6][i7] = view(i0,i1,i2,i3,i4,i5,i6,i7);
            break;
        }*/
        default:
        {
            cerr << "buffer_to_json(): unsupported number of dimensions: " << ndim << endl;
            exit(-5);
        }
    }
    return root;
}


Json::Value HDC::to_json(int mode) {
    Json::Value root;
    if (mode == 0) {
// //         switch(*type) {
// //             case(INT8_ID):
// //             {
// //                 root = buffer_to_json<int8_t>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //                 break;
// //             }
// //             case(INT16_ID):
// //             {
// //                 root =  buffer_to_json<int16_t>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //                 break;
// //             }
// //             case(INT32_ID):
// //             {
// //                 root =  buffer_to_json<int32_t>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //                 break;
// //             }
// //             /*case(INT64_ID):
// //             {
// //             root =  buffer_to_json<int64_t>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //                 break;
// //             }*/
// //             case(UINT8_ID):
// //             {
// //                 root = buffer_to_json<uint8_t>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //                 break;
// //             }
// //             case(UINT16_ID):
// //             {
// //                 root =  buffer_to_json<uint16_t>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //                 break;
// //             }
// //             case(UINT32_ID):
// //             {
// //                 root =  buffer_to_json<uint32_t>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //                 break;
// //             }
// //             /*case(UINT64_ID):
// //              {  *
// //              root =  buffer_to_json<uint64_t>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //              break;
// //             }*/
// //             case(FLOAT_ID):
// //             {
// //                 root =  buffer_to_json<float>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //                 break;
// //             }
// //             case(DOUBLE_ID):
// //             {
// //                 root =  buffer_to_json<double>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //                 break;
// //             }
// //             case(STRUCT_ID):
// //             {
// //                 for (auto it : *children) {
// //                     HDC* node = (HDC*)(storage->get(it.address.c_str()));
// //                     root[it.key.c_str()] = node->to_json(mode);
// //                 }
// //                 break;
// //             }
// //             case(LIST_ID):
// //             {
// //                 Json::Value elements;
// //                 for (size_t i=0;i<children->size();i++)
// //                     root.append(((HDC*)(storage->get(children->get<by_index>()[i].address.c_str())))->to_json(mode));
// //                 break;
// //             }
// //             case(EMPTY_ID):
// //             {
// //                 root = Json::nullValue;
// //                 break;
// //             }
// //             case(STRING_ID):
// //             {
// //                 root = as_string();
// //                 break;
// //             }
// //             case(BOOL_ID):
// //             {
// //                 root = buffer_to_json<bool>(buff_get_data_ptr(storage->get(uuid)),get_ndim(),get_shape());
// //                 break;
// //             }
// //             default:
// //             {
// //                 cerr << "to_json(): Type " << get_type_str() << " not supported yet.";
// //                 exit(-1);
// //             }
// //         }
    }
    else {
        cerr << "to_json(): Mode " << mode << " not supported yet.";
        exit(-1);
    }
    return root;
}

void HDC::to_json(string filename, int mode)
{
    #ifdef DEBUG
    cout << "Saving output JSON to " << filename << endl;
    #endif
    ofstream json_file;
    json_file.open(filename.c_str());
    json_file << this->to_json(mode);
    json_file.close();
    return;
}

/* Saves children to JSON strin in order to store tree hierarchy in KV stores*/
string map_to_json(map_t& children) {
        Json::Value root;
    //root["size"] = Json::UInt64(children.size());
    for (size_t i=0;i<children.size();i++) {
                    string key = children.get<by_index>()[i].key.c_str();
                    HDC* node = ((HDC*)(children.get<by_index>()[i].address.c_str()));
                    root[key] = node->get_uuid();
                }
    stringstream ss;
    ss << root;
    return ss.str();
}

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
