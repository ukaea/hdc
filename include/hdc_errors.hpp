#ifndef HDC_ERRORS_H
#define HDC_ERRORS_H


#include <exception>
#include <string>
class HDCException: public std::exception {
private:
    std::string message_;
public:
    explicit HDCException(const std::string& message);
    explicit HDCException();
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};

class HDCBadAllocException: public std::bad_alloc {
private:
    std::string message_;
public:
    explicit HDCBadAllocException(const std::string& message);
    explicit HDCBadAllocException();
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};
#endif //HDC_ERRORS_H
