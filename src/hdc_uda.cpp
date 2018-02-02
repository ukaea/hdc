#include "hdc_uda.h"
#include <uda/uda.h>
#include <c++/UDA.hpp>
//TODO: Make this static method
HDC from_uda(const std::string& signalName, const std::string& dataSource) {
    uda::Client client;
    const uda::Result& result = client.get(signalName,dataSource);
    uda::Data* data = result.data();
    std::cout << "size       : " << data->size() << std::endl;
    std::cout << "type       : " << data->type().name() << std::endl;
    std::cout << "byte_length: " << data->byte_length() << std::endl;
    TypeID t = to_typeid(data->type());
    HDC h;
    return h;
}
