#include "hdc_errors.hpp"

HDCException::HDCException() {}
HDCException::HDCException(const std::string& message) : message_(message) {}
HDCBadAllocException::HDCBadAllocException() {}
HDCBadAllocException::HDCBadAllocException(const std::string& message) : message_(message) {}
