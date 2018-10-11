#include "hdc.hpp"

#include <sstream>

bool is_all_numeric(const Json::Value& root)
{
    bool ok = true;
    for (unsigned int i = 0; i < root.size(); i++) {
        if (!root[i].isNumeric() && !root[i].isArray()) {
            ok = false;
            break;
        }
        if (root.isArray()) {
            if (!is_all_numeric(root[i])) {
                ok = false;
                break;
            }
        }
    }
    return ok;
}

bool is_all_bool(const Json::Value& root)
{
    bool ok = true;
    for (unsigned int i = 0; i < root.size(); i++) {
        if (!root[i].isBool() && !root[i].isArray()) {
            ok = false;
            break;
        }
        if (root.isArray()) {
            if (!is_all_bool(root[i])) {
                ok = false;
                break;
            }
        }
    }
    return ok;
}

bool is_double(const Json::Value& root)
{
    if (!is_all_numeric(root)) return false;
    if (root.isDouble() && !root.isInt64() && !root.isInt() && !root.isUInt64() && !root.isUInt() &&
        !root.isBool()) {
            return true;
    } else if (root.isArray()) {
        for (unsigned int i = 0; i < root.size(); i++) {
            if (is_double(root[i])) return true;
        }
    } else { return false; }
    return false;
}

bool is_int(const Json::Value& root)
{
    if (!is_all_numeric(root)) return false;
    if ((root.isInt64() || root.isInt() || root.isUInt() || root.isUInt64())) { return true; }
    else if (root.isArray()) {
        for (unsigned int i = 0; i < root.size(); i++) {
            if (is_int(root[i])) return true;
        }
    } else { return false; }
    return false;
}

bool is_bool(const Json::Value& root)
{
    if (!is_all_numeric(root)) return false;
    if (root.isBool() && !root.isInt64() && !root.isInt() && !root.isUInt() && !root.isUInt64() &&
        !root.isDouble()) {
            return true;
    } else if (root.isArray()) {
        for (unsigned int i = 0; i < root.size(); i++) {
            if (is_double(root[i])) return true;
        }
    } else { return false; }
    return false;
}

bool is_jagged(const Json::Value& root)
{
    if (!root.isArray()) return false;
    unsigned int dim = 0;
    bool jagged = false;
    dim = root[0].size();
    for (unsigned int i = 0; i < root.size(); i++) {
        if (root[i].size() != dim) {
            return true;
            break;
        } else { return is_jagged(root[i]); }
    }
    return jagged;
}

std::vector<size_t> get_shape(const Json::Value& root)
{
    if (!root.isArray()) return {0};
    unsigned int dim = 0;
    size_t shape[HDC_MAX_DIMS];
    Json::Value curr = root;
    while (curr.isArray()) {
        shape[dim] = curr.size();
        curr = curr[0];
        dim++;
    }
    D(
        std::cout << "Dimension: " << dim << std::endl;
        std::cout << "Shape: (" << shape[0];
        for (unsigned int i = 1; i < dim; i++) std::cout << ", " << shape[i];
        std::cout << ")" << std::endl;
    )
    std::vector<size_t> res(dim);
    for (unsigned int i = 0; i < dim; i++) res[i] = shape[i];
    return res;
}


int get_rank(const Json::Value& root)
{
    if (!root.isArray()) return 0;
    int dim = 0;
    Json::Value curr = root;
    while (curr.isArray()) {
        curr = curr[0];
        dim++;
    }
    return dim;
}

HDC HDC::json_to_HDC(const ::Json::Value& root)
{
    HDC tree;
    switch (root.type()) {
        {
            case (Json::nullValue):
                DEBUG_STDOUT("root is null");
            tree.set_type(HDC_EMPTY);
            break;
        }
        case (Json::intValue): {
            DEBUG_STDOUT("root is int, value = " + to_string(root.asInt()));
            tree.set_data<int32_t>(root.asInt());
            break;
        }
        case (Json::uintValue): {
            DEBUG_STDOUT("root is uint, value = " + to_string(root.asUInt()));
            tree.set_data<uint32_t>(root.asUInt());
            break;
        }
        case (Json::realValue): {
            DEBUG_STDOUT("root is double, value = " + to_string(root.asDouble()));
            tree.set_data(root.asDouble());
            break;
        }
        case (Json::stringValue): {
            DEBUG_STDOUT("root is string, value = " + string(root.asString()));
            tree.set_string(root.asString());
            break;
        }
        case (Json::booleanValue): {
            DEBUG_STDOUT("root is bool, value = " + to_string(root.asBool()));
            tree.set_data<bool>(root.asBool());
            break;
        }
        case (Json::arrayValue): {
            DEBUG_STDOUT("root is array, size = " + to_string(root.size()));
            if (is_all_numeric(root) || is_all_bool(root)) {
                int8_t rank = ::get_rank(root);
                if (rank > HDC_MAX_DIMS) {
                    throw HDCException(
                            "json_to_hdc(): Unsupported number of dimensions: " + std::to_string(rank) + "\n");
                }
                auto shape = ::get_shape(root);
                hdc_type_t dt;
                if (is_double(root)) { dt = HDC_DOUBLE; }
                else if (is_int(root)) { dt = HDC_INT32; }
                else { dt = HDC_BOOL; }
                HDC d(shape, dt);
                tree = d;
                void* data_ptr = tree.as<void*>();
                if (dt == HDC_DOUBLE) {
                    switch (rank) {
                        case 1: {
                            andres::View<double> view(&shape[0], &shape[0] + 1, (double*)data_ptr);
                            for (unsigned int i = 0; i < shape[0]; i++) {
                                view(i) = root[i].asDouble();
                            }
                            break;
                        }
                        case 2: {
                            andres::View<double> view(&shape[0], &shape[0] + 2, (double*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    view(i, j) = root[i][j].asDouble();
                            break;
                        }
                        case 3: {
                            andres::View<double> view(&shape[0], &shape[0] + 3, (double*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    for (unsigned int k = 0; k < shape[2]; k++)
                                        view(i, j, k) = root[i][j][k].asDouble();
                            break;
                        }
                        case 4: {
                            andres::View<double> view(&shape[0], &shape[0] + 4, (double*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    for (unsigned int k = 0; k < shape[2]; k++)
                                        for (unsigned int l = 0; l < shape[3]; l++)
                                            view(i, j, k, l) = root[i][j][k][l].asDouble();
                            break;
                        }
                        case 5: {
                            andres::View<double> view(&shape[0], &shape[0] + 5, (double*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    for (unsigned int k = 0; k < shape[2]; k++)
                                        for (unsigned int l = 0; l < shape[3]; l++)
                                            for (unsigned int m = 0; m < shape[4]; m++)
                                                view(i, j, k, l, m) = root[i][j][k][l][m].asDouble();
                            break;
                        }
                        default: {
                            throw HDCException(
                                    "json_to_hdc(): Requested number of dimensions: " + std::to_string(rank) +
                                    " not implemented yet\n");
                        }
                    }
                } else if (dt == HDC_BOOL) {
                    switch (rank) {
                        case 1: {
                            andres::View<bool> view(&shape[0], &shape[0] + 1, (bool*)data_ptr);
                            for (unsigned int i = 0; i < shape[0]; i++) {
                                view(i) = root[i].asBool();
                            }
                            break;
                        }
                        case 2: {
                            andres::View<bool> view(&shape[0], &shape[0] + 2, (bool*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    view(i, j) = root[i][j].asBool();
                            break;
                        }
                        case 3: {
                            andres::View<bool> view(&shape[0], &shape[0] + 3, (bool*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    for (unsigned int k = 0; k < shape[2]; k++)
                                        view(i, j, k) = root[i][j][k].asBool();
                            break;
                        }
                        case 4: {
                            andres::View<bool> view(&shape[0], &shape[0] + 4, (bool*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    for (unsigned int k = 0; k < shape[2]; k++)
                                        for (unsigned int l = 0; l < shape[3]; l++)
                                            view(i, j, k, l) = root[i][j][k][l].asBool();
                            break;
                        }
                        case 5: {
                            andres::View<bool> view(&shape[0], &shape[0] + 5, (bool*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    for (unsigned int k = 0; k < shape[2]; k++)
                                        for (unsigned int l = 0; l < shape[3]; l++)
                                            for (unsigned int m = 0; m < shape[4]; m++)
                                                view(i, j, k, l, m) = root[i][j][k][l][m].asBool();
                            break;
                        }
                        default: {
                            throw HDCException(
                                    "json_to_hdc(): Requested number of dimensions: " + std::to_string(rank) +
                                    " not implemented yet\n");
                        }
                    }
                } else {
                    switch (rank) {
                        case 1: {
                            andres::View<int32_t> view(&shape[0], &shape[0] + 1, (int32_t*)data_ptr);
                            for (unsigned int i = 0; i < shape[0]; i++) view(i) = root[i].asInt();
                            break;
                        }
                        case 2: {
                            andres::View<int32_t> view(&shape[0], &shape[0] + 2, (int32_t*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    view(i, j) = root[i][j].asInt();
                            break;
                        }
                        case 3: {
                            andres::View<int32_t> view(&shape[0], &shape[0] + 3, (int32_t*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    for (unsigned int k = 0; k < shape[2]; k++)
                                        view(i, j, k) = root[i][j][k].asInt();
                            break;
                        }
                        case 4: {
                            andres::View<int32_t> view(&shape[0], &shape[0] + 4, (int32_t*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    for (unsigned int k = 0; k < shape[2]; k++)
                                        for (unsigned int l = 0; l < shape[3]; l++)
                                            view(i, j, k, l) = root[i][j][k][l].asInt();
                            break;
                        }
                        case 5: {
                            andres::View<int32_t> view(&shape[0], &shape[0] + 5, (int32_t*)data_ptr, andres::FirstMajorOrder);
                            for (unsigned int i = 0; i < shape[0]; i++)
                                for (unsigned int j = 0; j < shape[1]; j++)
                                    for (unsigned int k = 0; k < shape[2]; k++)
                                        for (unsigned int l = 0; l < shape[3]; l++)
                                            for (unsigned int m = 0; m < shape[4]; m++)
                                                view(i, j, k, l, m) = root[i][j][k][l][m].asInt();
                            break;
                        }
                        default: {
                            throw HDCException(
                                    "json_to_hdc(): Requested number of dimensions: " + std::to_string(rank) +
                                    " not implemented yet\n");
                        }
                    }
                }
            } else {
                // call recursively -- save list
                tree.set_type(HDC_LIST);
                for (unsigned int i = 0; i < root.size(); i++) {
                    HDC h = HDC::json_to_HDC(root[i]);
                    tree.append(h);
                }
            }
            break;
        }
        case (Json::objectValue): {
            DEBUG_STDOUT("root is object, children:\n");
            for (auto it = root.begin(); it != root.end(); ++it) {
                DEBUG_STDOUT("KEY: " + it.key().asString());
                HDC h = json_to_HDC(*it);
                tree.add_child(it.key().asCString(), h);
            }
            break;
        }
        default: throw HDCException("Bad JSON type");
    }
    return tree;
}

template <typename T>
Json::Value buffer_to_json(char* buffer, std::vector<size_t> shape, bool fortranOrder = false)
{
    // TODO: Add Fortran column order
    auto rank = shape.size();
    Json::Value root;
    andres::CoordinateOrder order;
    if (fortranOrder) {
        order = andres::LastMajorOrder;
    } else {
        order = andres::FirstMajorOrder;
    }
    andres::View<T> view(&shape[0], &shape[0] + rank, (T*)buffer, order);
    //TODO add fortran - C order switch
    switch (rank) {
        case (0): {
            root = ((T*)buffer)[0];
            break;
        }
        case (1): {
            for (unsigned int i0 = 0; i0 < shape[0]; i0++)
                root[i0] = (T)view(i0);
            break;
        }
        case (2): {
            for (unsigned int i0 = 0; i0 < shape[0]; i0++)
                for (unsigned int i1 = 0; i1 < shape[1]; i1++)
                    root[i0][i1] = view(i0, i1);
            break;
        }
        case (3): {
            for (unsigned int i0 = 0; i0 < shape[0]; i0++)
                for (unsigned int i1 = 0; i1 < shape[1]; i1++)
                    for (unsigned int i2 = 0; i2 < shape[2]; i2++)
                        root[i0][i1][i2] = view(i0, i1, i2);
            break;
        }
        case (4): {
            for (unsigned int i0 = 0; i0 < shape[0]; i0++)
                for (unsigned int i1 = 0; i1 < shape[1]; i1++)
                    for (unsigned int i2 = 0; i2 < shape[2]; i2++)
                        for (unsigned int i3 = 0; i3 < shape[3]; i3++)
                            root[i0][i1][i2][i3] = view(i0, i1, i2, i3);
            break;
        }
        case (5): {
            for (unsigned int i0 = 0; i0 < shape[0]; i0++)
                for (unsigned int i1 = 0; i1 < shape[1]; i1++)
                    for (unsigned int i2 = 0; i2 < shape[2]; i2++)
                        for (unsigned int i3 = 0; i3 < shape[3]; i3++)
                            for (unsigned int i4 = 0; i4 < shape[4]; i4++)
                                root[i0][i1][i2][i3][i4] = view(i0, i1, i2, i3, i4);
            break;
        }
            /*
            case (6):
            {
                for (unsigned int i0=0;i0<shape[0];i0++)
                    for (unsigned int i1=0;i1<shape[1];i1++)
                        for (unsigned int i2=0;i2<shape[2];i2++)
                            for (unsigned int i3=0;i3<shape[3];i3++)
                                for (unsigned int i4=0;i4<shape[4];i4++)
                                    for (unsigned int i5=0;i5<shape[5];i5++)
                                        root[i0][i1][i2][i3][i4][i5] = view(i0,i1,i2,i3,i4,i5);
                break;
            }
            case (7):
            {
                for (unsigned int i0=0;i0<shape[0];i0++)
                    for (unsigned int i1=0;i1<shape[1];i1++)
                        for (unsigned int i2=0;i2<shape[2];i2++)
                            for (unsigned int i3=0;i3<shape[3];i3++)
                                for (unsigned int i4=0;i4<shape[4];i4++)
                                    for (unsigned int i5=0;i5<shape[5];i5++)
                                        for (unsigned int i6=0;i6<shape[5];i6++)
                                            root[i0][i1][i2][i3][i4][i5][i6] = view(i0,i1,i2,i3,i4,i5,i6);
                break;
            }
            case (8):
            {
                for (unsigned int i0=0;i0<shape[0];i0++)
                    for (unsigned int i1=0;i1<shape[1];i1++)
                        for (unsigned int i2=0;i2<shape[2];i2++)
                            for (unsigned int i3=0;i3<shape[3];i3++)
                                for (unsigned int i4=0;i4<shape[4];i4++)
                                    for (unsigned int i5=0;i5<shape[5];i5++)
                                        for (unsigned int i6=0;i6<shape[5];i6++)
                                            for (unsigned int i7=0;i7<shape[5];i7++)
                                                root[i0][i1][i2][i3][i4][i5][i6][i7] = view(i0,i1,i2,i3,i4,i5,i6,i7);
                break;
            }*/
        default: {
            throw HDCException("buffer_to_json(): unsupported number of dimensions: " + std::to_string(rank) + "\n");
        }
    }
    return root;
}


Json::Value HDC::to_json(int mode) const
{
    hdc_header_t header = get_header();
    Json::Value root;
    if (mode == 0) {
        switch (header.type) {
            case (HDC_INT8): {
                root = buffer_to_json<int8_t>(get_data_ptr(), get_shape(), is_fortranorder());
                break;
            }
            case (HDC_INT16): {
                root = buffer_to_json<int16_t>(get_data_ptr(), get_shape(), is_fortranorder());
                break;
            }
            case (HDC_INT32): {
                root = buffer_to_json<int32_t>(get_data_ptr(), get_shape(), is_fortranorder());
                break;
            }
            case (HDC_INT64): {
                root = buffer_to_json<int64_t>(get_data_ptr(), get_shape(), is_fortranorder());
                break;
            }
            case (HDC_UINT8): {
                root = buffer_to_json<uint8_t>(get_data_ptr(), get_shape(), is_fortranorder());
                break;
            }
            case (HDC_UINT16): {
                root = buffer_to_json<uint16_t>(get_data_ptr(), get_shape(), is_fortranorder());
                break;
            }
            case (HDC_UINT32): {
                root = buffer_to_json<uint32_t>(get_data_ptr(), get_shape(), is_fortranorder());
                break;
            }
                /*case(HDC_UINT64):
                 {  *
                 root =  buffer_to_json<uint64_t>(get_data_ptr(),get_rank(),get_shape(),is_fortranorder());
                 break;
                }*/
            case (HDC_FLOAT): {
                root = buffer_to_json<float>(get_data_ptr(), get_shape(), is_fortranorder());
                break;
            }
            case (HDC_DOUBLE): {
                root = buffer_to_json<double>(get_data_ptr(), get_shape(), is_fortranorder());
                break;
            }
            case (HDC_STRUCT): {
                auto children = get_children_ptr();
                for (auto it = children->begin(); it != children->end(); ++it) {
                    HDC node(storage, it->address.c_str());
                    root[it->key.c_str()] = node.to_json(mode);
                }
                break;
            }
            case (HDC_LIST): {
                root = Json::arrayValue;
                for (size_t i = 0; i < this->children_count(); i++) {
                    root[static_cast<int>(i)] = get_single(i).to_json(mode);
                }
                break;
            }
            case (HDC_EMPTY): {
                root = Json::nullValue;
                break;
            }
            case (HDC_STRING): {
                root = as_string();
                break;
            }
            case (HDC_BOOL): {
                root = buffer_to_json<bool>(get_data_ptr(), get_shape(), is_fortranorder());
                break;
            }
            default: {
                throw HDCException("to_json(): Type " + std::string(get_type_str()) + " not supported yet.");
            }
        }
    } else {
        throw HDCException("to_json(): Mode " + std::to_string(mode) + " not supported yet.\n");
    }
    return root;
}

void HDC::to_json(string filename, int mode) const
{
    DEBUG_STDOUT("Saving output JSON to " + filename);
    ofstream json_file;
    json_file.open(filename.c_str());
    json_file << this->to_json(mode);
    json_file.close();
    return;
}

string HDC::to_json_string(int mode) const
{
    Json::StreamWriterBuilder writerBuilder;
    std::ostringstream oss;
    writerBuilder.newStreamWriter()->write(this->to_json(mode), &oss);
    return oss.str();
}

HDC HDC::from_json_string(const std::string& json_string)
{
    HDC tree;
    stringstream file = stringstream(json_string);
    file.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        Json::Value root;
        file >> root;
        tree = HDC::json_to_HDC(root);
    }
    catch (ifstream::failure e) {
        cout << "Error reading JSON string." << endl;
    }
    return tree;
}

/* Saves children to JSON strin in order to store tree hierarchy in KV stores*/
string HDC::hdc_map_to_json(hdc_map_t& children)
{
    Json::Value root;
    //root["size"] = Json::UInt64(children.size());
    for (size_t i = 0; i < children.size(); i++) {
        string key = children.get<by_index>()[i].key.c_str();
        HDC* node = ((HDC*)(children.get<by_index>()[i].address.c_str()));
        root[key] = node->get_uuid();
    }
    stringstream ss;
    ss << root;
    return ss.str();
}

HDC HDC::from_json(const string& filename, const string& datapath)
{
    HDC tree;
    ifstream file;
    file.exceptions(ifstream::failbit | ifstream::badbit);
    Json::Value root;
    try {
        file.open(filename);
        file >> root;
        if (datapath != "") {
            auto split_path = split(datapath);
            for (auto& k : split_path) {
                if (k.type() == typeid(size_t)) {
                    root = root[static_cast<int>(boost::get<size_t>(k))];
                } else {
                    root = root[boost::get<std::string>(k)];
                }
            }
        }
        tree = HDC::json_to_HDC(root);
    }
    catch (ifstream::failure e) {
        cout << "Error reading / opening file." << endl;
    }
    file.close();
    return tree;
}
