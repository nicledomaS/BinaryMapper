#pragma once

#include <map>
#include <string>
#include <memory>
#include <functional>

class BinaryObject;
struct BinaryData;

enum class BinaryType;
using BinaryObjectFactory = std::function<std::shared_ptr<BinaryObject>(std::unique_ptr<BinaryData> binaryData)>;

class BinaryMapper
{
public:
    std::shared_ptr<BinaryObject> create(std::unique_ptr<BinaryData> binaryData);
    void destroy(const std::string& name);

    std::shared_ptr<BinaryObject> find(const std::string& name);
    
    void addFactory(BinaryType type, BinaryObjectFactory factory);

private:
    std::map<BinaryType, BinaryObjectFactory> m_factories;
    std::map<std::string, std::shared_ptr<BinaryObject>> m_wrappers;
};