#include "hdc_utils.h"
#include <tuple>
#include <boost/tokenizer.hpp>
#include <boost/variant.hpp>
#include <typeinfo>


/* ------------------------- UUID generation ----------------------------- */

boost::mt19937 ran;

void print_uuid(std::vector<char> uuid) {
    for (int i = 0; i < 16; ++i)
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (uint)uuid[i] << " ";
    std::cout << std::endl;
    printf("\n");
}

std::vector<char> generate_uuid() {
    boost::uuids::random_generator gen(&ran);
    boost::uuids::uuid u = gen();
    std::vector<char> v(u.size());
    copy(u.begin(), u.end(), v.begin());
    return v;
}

std::string generate_uuid_str() {
    boost::uuids::random_generator gen(&ran);
    boost::uuids::uuid u = gen();
    const std::string str = boost::lexical_cast<std::string>(u);
    return str;
}

/* -------------------------  String manipulation ----------------------------- */

std::vector <boost::variant<size_t,std::string>> split(const std::string& s) {
    std::vector<boost::variant<size_t,std::string>> parts;
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    boost::char_separator<char> sep("/]", "[");
    tokenizer tok{s, sep};
    bool is_index = false;
    bool was_bracket = false;
    for (const auto &t : tok) {
        was_bracket = (t.operator[](0) == '[');
        if (was_bracket) {
            is_index = true;
            continue;
        }
        boost::variant<size_t,std::string> var;
        if (is_index) var = boost::lexical_cast<size_t>(t);
        else var = t;
        parts.push_back(var);
        if (is_index) is_index = false;
    }
    return parts;
}

/* -------------------------  Types Definitions  ------------------------- */


size_t hdc_sizeof (TypeID type) {
    switch(type) {
        case EMPTY_ID:
        case LIST_ID:
        case STRUCT_ID:
        case ERROR_ID:
            return 0;
        case STRING_ID:
            return sizeof(char);
        case UINT8_ID:
        case INT8_ID:
            return sizeof(int8_t);
        case UINT16_ID:
        case INT16_ID:
            return sizeof(int16_t);
        case UINT32_ID:
        case INT32_ID:
            return sizeof(int32_t);
        case UINT64_ID:
        case INT64_ID:
            return sizeof(int64_t);
        case FLOAT_ID:
            return sizeof(float);
        case DOUBLE_ID:
            return sizeof(double);
        case BOOL_ID:
            return sizeof(bool);
        default:
            throw HDCException("hdc_sizeof(): Wrong type "+std::to_string(type)+"\n");
    }
};

bool hdc_is_primitive_type(TypeID type) {
    switch(type) {
        case EMPTY_ID:
        case LIST_ID:
        case STRUCT_ID:
        case ERROR_ID:
            return false;
        case STRING_ID:
        case UINT8_ID:
        case INT8_ID:
        case UINT16_ID:
        case INT16_ID:
        case UINT32_ID:
        case INT32_ID:
        case UINT64_ID:
        case INT64_ID:
        case FLOAT_ID:
        case DOUBLE_ID:
        case BOOL_ID:
            return true;
        default:
            throw HDCException("hdc_sizeof(): Wrong type "+std::to_string(type)+"\n");
    }
}

std::string hdc_type_str(TypeID _type) {
    switch(_type) {
        case EMPTY_ID:
            return "null";
        case LIST_ID:
            return "list";
        case STRUCT_ID:
            return "hdc";
        case ERROR_ID:
            return "error";
        case STRING_ID:
            return "string";
        case UINT8_ID:
            return "uint8";
        case INT8_ID:
            return "int8";
        case UINT16_ID:
            return "uint16";
        case INT16_ID:
            return "int16";
        case UINT32_ID:
            return "uint32";
        case INT32_ID:
            return "int32";
        case UINT64_ID:
            return "uint64";
        case INT64_ID:
            return "int64";
        case FLOAT_ID:
            return "float32";
        case DOUBLE_ID:
            return "float64";
        case BOOL_ID:
            return "bool";
        default:
            return "unknown";
    }
}

template <typename T>
TypeID to_typeid(T a) {
    throw HDCException("hdc_sizeof(): Wrong type "+std::to_string(a)+"\n");
};
TypeID to_typeid(double a) {return DOUBLE_ID;};
TypeID to_typeid(float a) {return FLOAT_ID;};
TypeID to_typeid(int64_t a) {return INT64_ID;};
TypeID to_typeid(int32_t a) {return INT32_ID;};
TypeID to_typeid(int16_t a) {return INT16_ID;};
TypeID to_typeid(int8_t a) {return INT8_ID;};
TypeID to_typeid(char a) {return INT8_ID;};
TypeID to_typeid(uint64_t a) {return UINT64_ID;};
TypeID to_typeid(uint32_t a) {return UINT32_ID;};
TypeID to_typeid(uint16_t a) {return UINT16_ID;};
TypeID to_typeid(uint8_t a) {return UINT8_ID;};
TypeID to_typeid(std::string a) {return STRING_ID;};
TypeID to_typeid(char* a) {return STRING_ID;};
TypeID to_typeid(char const* a) {return STRING_ID;};
TypeID to_typeid(bool a) {return BOOL_ID;};
TypeID numpy_format_to_typeid(std::string format, size_t itemsize) {
    if (format == "i") {
        return INT32_ID;
    } else if (format == "d") {
        return DOUBLE_ID;
    } else if (format == "f") {
        return FLOAT_ID;
    } else if (format == "l") {
        return INT64_ID;
    } else if (format == "h") {
        return INT16_ID;
    // TODO crashed - bool binary compatible?
    // } else if (format == "?") {
    //     return BOOL_ID;
    } else if (format == "b") {
        return INT8_ID;
    }
    // return error by default
    return ERROR_ID;
};


TypeID to_typeid(const std::type_info& t) {
    const std::string type_str = t.name();
    if (t == typeid(std::string) || t == typeid(char) || t == typeid(char*)) {
        return STRING_ID;
    } else if (t == typeid(double)) {
        return DOUBLE_ID;
    } else if (t == typeid(int)) {
        return INT64_ID;
    } else if (t == typeid(short)) {
        return INT64_ID;
    } else {
        return ERROR_ID;
    }
}

TypeID uda_str_to_typeid(std::string& str) {
    if (str == "STRING") {
        return STRING_ID;
    } else if (str =="STRING *") {
        std::cerr << "Warning: STRING * type is not supported!\n";
        return LIST_ID;
    } else if (str =="int") {
        return INT64_ID;
    } else if (str =="short") {
        return INT64_ID;
    } else if (str =="double") {
        return DOUBLE_ID;
    }
    return ERROR_ID;
}

/* -------------------------  Buffer Manipulation  ------------------------- */

char* transpose_buffer(char* buffer, int8_t ndim, size_t* shape, TypeID type_, bool fortranOrder) {
    //TODO: remove these ugly switch - cases and use something better (if possible)
    auto item_size = hdc_sizeof(type_);
    auto n_items = 1lu;
    size_t new_shape[HDC_MAX_DIMS];
    memset(new_shape, 0, sizeof(size_t)*HDC_MAX_DIMS);
    for (int i=0; i<ndim;i++) {
        n_items *= shape[i];
        new_shape[i] = shape[ndim-i-1];
    }
    andres::CoordinateOrder order;
    andres::CoordinateOrder new_order;
    if (fortranOrder) {
        order = andres::LastMajorOrder;
        new_order = andres::FirstMajorOrder;
    }
    else {
        order = andres::FirstMajorOrder;
        new_order = andres::LastMajorOrder;
    }
    char* new_buffer = new char[item_size*n_items];

    switch(type_) {
        case(INT8_ID):
        {
            andres::View<int8_t> view(shape, shape+ndim, (int8_t*)buffer,order);
            andres::View<int8_t> new_view((size_t*)new_shape, new_shape+ndim, (int8_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(INT16_ID):
        {
            andres::View<int16_t> view(shape, shape+ndim, (int16_t*)buffer,order);
            andres::View<int16_t> new_view((size_t*)new_shape, new_shape+ndim, (int16_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(INT32_ID):
        {
            andres::View<int32_t> view(shape, shape+ndim, (int32_t*)buffer,order);
            andres::View<int32_t> new_view((size_t*)new_shape, new_shape+ndim, (int32_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
//             std::cout << view.asString() << std::endl;
//             andres::View<int32_t> transposed_view(shape, shape+ndim, (int32_t*)new_buffer,order);
//             std::cout << transposed_view.asString() << std::endl;
            break;
        }
        case(INT64_ID):
        {
            andres::View<int64_t> view(shape, shape+ndim, (int64_t*)buffer,order);
            andres::View<int64_t> new_view((size_t*)new_shape, new_shape+ndim, (int64_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(UINT8_ID):
        {
            andres::View<uint8_t> view(shape, shape+ndim, (uint8_t*)buffer,order);
            andres::View<uint8_t> new_view((size_t*)new_shape, new_shape+ndim, (uint8_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(UINT16_ID):
        {
            andres::View<uint16_t> view(shape, shape+ndim, (uint16_t*)buffer,order);
            andres::View<uint16_t> new_view((size_t*)new_shape, new_shape+ndim, (uint16_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(UINT32_ID):
        {
            andres::View<uint32_t> view(shape, shape+ndim, (uint32_t*)buffer,order);
            andres::View<uint32_t> new_view((size_t*)new_shape, new_shape+ndim, (uint32_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(UINT64_ID):
        {
            andres::View<uint64_t> view(shape, shape+ndim, (uint64_t*)buffer,order);
            andres::View<uint64_t> new_view((size_t*)new_shape, new_shape+ndim, (uint64_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(FLOAT_ID):
        {
            andres::View<float> view(shape, shape+ndim, (float*)buffer,order);
            andres::View<float> new_view((size_t*)new_shape, new_shape+ndim, (float*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(DOUBLE_ID):
        {
            andres::View<double> view(shape, shape+ndim, (double*)buffer,order);
            andres::View<double> new_view((size_t*)new_shape, new_shape+ndim, (double*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(BOOL_ID):
        {
            andres::View<bool> view(shape, shape+ndim, (bool*)buffer,order);
            andres::View<bool> new_view((size_t*)new_shape, new_shape+ndim, (bool*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(STRUCT_ID):
        case(LIST_ID):
        case(EMPTY_ID):
        case(STRING_ID):
            throw HDCException("transpose_buffer(): TypeID = "+std::to_string((size_t)type_)+" cannot be transposed.");
        default:
        {
            throw HDCException("transpose_buffer(): TypeID = "+std::to_string((size_t)type_)+" not supported yet.");
        }
    }

    return new_buffer;
}

char* transpose_buffer(char* buffer) {
    header_t header;
    memcpy(&header,buffer,sizeof(header_t));
    auto data = buffer+sizeof(header_t);
    char* new_buffer = new char[header.buffer_size];
    bool fortranOrder = (header.flags & HDCFortranOrder) == HDCFortranOrder;
    std::cout << "fortranOrder" << (int)fortranOrder<< std::endl;
    auto transposed_data = transpose_buffer(data, header.ndim, header.shape, (TypeID)header.type, fortranOrder);
    memcpy(new_buffer,&header,sizeof(header_t));
    memcpy(new_buffer,transposed_data,header.data_size);
    return new_buffer;
}
