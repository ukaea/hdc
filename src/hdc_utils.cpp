#include "hdc_utils.h"

#include <boost/tokenizer.hpp>
#include <boost/variant.hpp>
#include <tuple>
#include <typeinfo>

#include "hdc_helpers.h"


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

std::vector <boost::variant<size_t,std::string>> split_no_brackets(const std::string& s) {
    std::vector<boost::variant<size_t,std::string>> parts;
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    boost::char_separator<char> sep("/", "");
    tokenizer tok{s, sep};
    boost::variant<size_t,std::string> var;
    for (const auto &t : tok) {
        try {
            var = boost::lexical_cast<size_t>(t);
        } catch(boost::bad_lexical_cast) {
            var = t;
        }
        parts.push_back(var);
    }
    return parts;
}

/* -------------------------  Types Definitions  ------------------------- */


size_t hdc_sizeof (hdc_type_t type) {
    switch(type) {
        case HDC_EMPTY:
        case HDC_LIST:
        case HDC_STRUCT:
        case HDC_ERROR:
            return 0;
        case HDC_STRING:
            return sizeof(char);
        case HDC_UINT8:
        case HDC_INT8:
            return sizeof(int8_t);
        case HDC_UINT16:
        case HDC_INT16:
            return sizeof(int16_t);
        case HDC_UINT32:
        case HDC_INT32:
            return sizeof(int32_t);
        case HDC_UINT64:
        case HDC_INT64:
            return sizeof(int64_t);
        case HDC_FLOAT:
            return sizeof(float);
        case HDC_DOUBLE:
            return sizeof(double);
        case HDC_BOOL:
            return sizeof(bool);
        default:
            throw HDCException("hdc_sizeof(): Wrong type "+std::to_string(type)+"\n");
    }
};

bool hdc_is_primitive_type(hdc_type_t type) {
    switch(type) {
        case HDC_EMPTY:
        case HDC_LIST:
        case HDC_STRUCT:
        case HDC_ERROR:
            return false;
        case HDC_STRING:
        case HDC_UINT8:
        case HDC_INT8:
        case HDC_UINT16:
        case HDC_INT16:
        case HDC_UINT32:
        case HDC_INT32:
        case HDC_UINT64:
        case HDC_INT64:
        case HDC_FLOAT:
        case HDC_DOUBLE:
        case HDC_BOOL:
            return true;
        default:
            throw HDCException("hdc_sizeof(): Wrong type "+std::to_string(type)+"\n");
    }
}

std::string hdc_type_str(hdc_type_t _type) {
    switch(_type) {
        case HDC_EMPTY:
            return "null";
        case HDC_LIST:
            return "list";
        case HDC_STRUCT:
            return "hdc";
        case HDC_ERROR:
            return "error";
        case HDC_STRING:
            return "string";
        case HDC_UINT8:
            return "uint8";
        case HDC_INT8:
            return "int8";
        case HDC_UINT16:
            return "uint16";
        case HDC_INT16:
            return "int16";
        case HDC_UINT32:
            return "uint32";
        case HDC_INT32:
            return "int32";
        case HDC_UINT64:
            return "uint64";
        case HDC_INT64:
            return "int64";
        case HDC_FLOAT:
            return "float32";
        case HDC_DOUBLE:
            return "float64";
        case HDC_BOOL:
            return "bool";
        default:
            return "unknown";
    }
}

template <typename T>
hdc_type_t to_typeid(T a) {
    throw HDCException("hdc_sizeof(): Wrong type "+std::to_string(a)+"\n");
};
hdc_type_t to_typeid(double a UNUSED) {return HDC_DOUBLE;};
hdc_type_t to_typeid(float a UNUSED) {return HDC_FLOAT;};
hdc_type_t to_typeid(int64_t a UNUSED) {return HDC_INT64;};
hdc_type_t to_typeid(int32_t a UNUSED) {return HDC_INT32;};
hdc_type_t to_typeid(int16_t a UNUSED) {return HDC_INT16;};
hdc_type_t to_typeid(int8_t a UNUSED) {return HDC_INT8;};
hdc_type_t to_typeid(char a UNUSED) {return HDC_INT8;};
hdc_type_t to_typeid(uint64_t a UNUSED) {return HDC_UINT64;};
hdc_type_t to_typeid(uint32_t a UNUSED) {return HDC_UINT32;};
hdc_type_t to_typeid(uint16_t a UNUSED) {return HDC_UINT16;};
hdc_type_t to_typeid(uint8_t a UNUSED) {return HDC_UINT8;};
hdc_type_t to_typeid(std::string a UNUSED) {return HDC_STRING;};
hdc_type_t to_typeid(char* a UNUSED) {return HDC_STRING;};
hdc_type_t to_typeid(char const* a UNUSED) {return HDC_STRING;};
hdc_type_t to_typeid(bool a UNUSED) {return HDC_BOOL;};
hdc_type_t numpy_format_to_typeid(std::string format, size_t itemsize UNUSED) {
    if (format == "i") {
        return HDC_INT32;
    } else if (format == "d") {
        return HDC_DOUBLE;
    } else if (format == "f") {
        return HDC_FLOAT;
    } else if (format == "l") {
        return HDC_INT64;
    } else if (format == "h") {
        return HDC_INT16;
    // TODO crashed - bool binary compatible?
    // } else if (format == "?") {
    //     return HDC_BOOL;
    } else if (format == "b") {
        return HDC_INT8;
    }
    // return error by default
    return HDC_ERROR;
};


hdc_type_t to_typeid(const std::type_info& t) {
    const std::string type_str = t.name();
    if (t == typeid(std::string) || t == typeid(char) || t == typeid(char*)) {
        return HDC_STRING;
    } else if (t == typeid(double)) {
        return HDC_DOUBLE;
    } else if (t == typeid(int)) {
        return HDC_INT64;
    } else if (t == typeid(short)) {
        return HDC_INT64;
    } else {
        return HDC_ERROR;
    }
}

hdc_type_t uda_str_to_typeid(std::string& str) {
    if (str == "STRING") {
        return HDC_STRING;
    } else if (str =="STRING *") {
        std::cerr << "Warning: STRING * type is not supported!\n";
        return HDC_LIST;
    } else if (str =="int") {
        return HDC_INT64;
    } else if (str =="short") {
        return HDC_INT64;
    } else if (str =="double") {
        return HDC_DOUBLE;
    }
    return HDC_ERROR;
}

/* -------------------------  Buffer Manipulation  ------------------------- */

char* transpose_buffer(char* buffer, int8_t ndim, size_t* shape, hdc_type_t type_, bool fortranOrder) {
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
        case(HDC_INT8):
        {
            andres::View<int8_t> view(shape, shape+ndim, (int8_t*)buffer,order);
            andres::View<int8_t> new_view((size_t*)new_shape, new_shape+ndim, (int8_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(HDC_INT16):
        {
            andres::View<int16_t> view(shape, shape+ndim, (int16_t*)buffer,order);
            andres::View<int16_t> new_view((size_t*)new_shape, new_shape+ndim, (int16_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(HDC_INT32):
        {
            andres::View<int32_t> view(shape, shape+ndim, (int32_t*)buffer,order);
            andres::View<int32_t> new_view((size_t*)new_shape, new_shape+ndim, (int32_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
//             std::cout << view.asString() << std::endl;
//             andres::View<int32_t> transposed_view(shape, shape+ndim, (int32_t*)new_buffer,order);
//             std::cout << transposed_view.asString() << std::endl;
            break;
        }
        case(HDC_INT64):
        {
            andres::View<int64_t> view(shape, shape+ndim, (int64_t*)buffer,order);
            andres::View<int64_t> new_view((size_t*)new_shape, new_shape+ndim, (int64_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(HDC_UINT8):
        {
            andres::View<uint8_t> view(shape, shape+ndim, (uint8_t*)buffer,order);
            andres::View<uint8_t> new_view((size_t*)new_shape, new_shape+ndim, (uint8_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(HDC_UINT16):
        {
            andres::View<uint16_t> view(shape, shape+ndim, (uint16_t*)buffer,order);
            andres::View<uint16_t> new_view((size_t*)new_shape, new_shape+ndim, (uint16_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(HDC_UINT32):
        {
            andres::View<uint32_t> view(shape, shape+ndim, (uint32_t*)buffer,order);
            andres::View<uint32_t> new_view((size_t*)new_shape, new_shape+ndim, (uint32_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(HDC_UINT64):
        {
            andres::View<uint64_t> view(shape, shape+ndim, (uint64_t*)buffer,order);
            andres::View<uint64_t> new_view((size_t*)new_shape, new_shape+ndim, (uint64_t*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(HDC_FLOAT):
        {
            andres::View<float> view(shape, shape+ndim, (float*)buffer,order);
            andres::View<float> new_view((size_t*)new_shape, new_shape+ndim, (float*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(HDC_DOUBLE):
        {
            andres::View<double> view(shape, shape+ndim, (double*)buffer,order);
            andres::View<double> new_view((size_t*)new_shape, new_shape+ndim, (double*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(HDC_BOOL):
        {
            andres::View<bool> view(shape, shape+ndim, (bool*)buffer,order);
            andres::View<bool> new_view((size_t*)new_shape, new_shape+ndim, (bool*)new_buffer,new_order);
            for (size_t i=0;i<view.size();i++) new_view(i) = view(i);
            break;
        }
        case(HDC_STRUCT):
        case(HDC_LIST):
        case(HDC_EMPTY):
        case(HDC_STRING):
            throw HDCException("transpose_buffer(): hdc_type_t = "+std::to_string((size_t)type_)+" cannot be transposed.");
        default:
        {
            throw HDCException("transpose_buffer(): hdc_type_t = "+std::to_string((size_t)type_)+" not supported yet.");
        }
    }

    return new_buffer;
}

char* transpose_buffer(char* buffer) {
    hdc_header_t header;
    memcpy(&header,buffer,sizeof(hdc_header_t));
    auto data = buffer+sizeof(hdc_header_t);
    char* new_buffer = new char[header.buffer_size];
    bool fortranOrder = (header.flags & HDCFortranOrder) == HDCFortranOrder;
    std::cout << "fortranOrder" << (int)fortranOrder<< std::endl;
    auto transposed_data = transpose_buffer(data, header.ndim, header.shape, (hdc_type_t)header.type, fortranOrder);
    memcpy(new_buffer,&header,sizeof(hdc_header_t));
    memcpy(new_buffer,transposed_data,header.data_size);
    return new_buffer;
}


/**
 * Check if a file exists
 * @return true if and only if the file exists, false else
 */
bool fileExists(const std::string& file) {
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
}
