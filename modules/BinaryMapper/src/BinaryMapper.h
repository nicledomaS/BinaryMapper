#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <functional>

class BinaryObject;

namespace binary_data
{

struct BinaryData;
enum class BinaryType;

} // binary_data


using BinaryObjectFactory = std::function<std::shared_ptr<BinaryObject>(std::unique_ptr<binary_data::BinaryData>)>;

class BinaryMapper
{
public:
    std::shared_ptr<BinaryObject> create(std::unique_ptr<binary_data::BinaryData> binaryData);
    void destroy(const std::string& name);

    std::shared_ptr<BinaryObject> find(const std::string& name);
    
    void addFactory(binary_data::BinaryType type, BinaryObjectFactory factory);

    std::vector<std::string> getBinaryNames() const;
    std::vector<std::string> getFactoryNames() const;

private:
    std::map<binary_data::BinaryType, BinaryObjectFactory> m_factories;
    std::map<std::string, std::shared_ptr<BinaryObject>> m_wrappers;
};

std::unique_ptr<BinaryMapper> createMapper();