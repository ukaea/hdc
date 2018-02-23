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

std::vector <boost::variant<size_t,std::string>> split(std::string s) {
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
