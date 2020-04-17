#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

class HDC;

namespace hdc {
namespace serialization {

class Serialiser;

class SerialiserMap {
public:
    SerialiserMap() noexcept;
    Serialiser& operator[](const std::string& key) const;
    std::vector<std::string> available_serializers() const;
private:
    std::unordered_map<std::string, std::unique_ptr<Serialiser>> serializers_;
};

class Serialiser {
public:
    virtual ~Serialiser() = default;

    virtual void serialize(const HDC& hdc, const std::string& filename, const std::string& datapath) = 0;
    virtual HDC deserialize(const std::string& filename, const std::string& datapath) = 0;

    virtual std::string to_string(const HDC& hdc) = 0;
    virtual HDC from_string(const std::string& string) = 0;

    static Serialiser& find_serializer(const std::string& protocol);
    static std::vector<std::string> available_serializers();

private:
    static const SerialiserMap serializers_;
};

}}
