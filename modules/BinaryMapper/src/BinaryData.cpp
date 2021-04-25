#include "BinaryData.h"

#include <algorithm>
#include <map>

namespace binary_data
{

namespace
{

constexpr auto UnknownStr = "Unknown";
constexpr auto ExecutableStr = "Executable";
constexpr auto LibraryStr = "Library";
constexpr auto ModuleStr = "Module";

static std::map<std::string, BinaryType> binaryTypes = {
    { ExecutableStr, BinaryType::Executable },
    { LibraryStr, BinaryType::Library },
    { ModuleStr, BinaryType::Module }
};

} // anonymous

std::string toString(BinaryType type)
{
    auto it = find_if(binaryTypes.begin(), binaryTypes.end(), [type](const auto& pair)
    {
        return pair.second == type;
    });

    return it == binaryTypes.end() ? UnknownStr : it->first;
}

BinaryType fromString(const std::string& value)
{
    auto it = binaryTypes.find(value);
    return it == binaryTypes.end() ? BinaryType::Unknown : it->second;
}

} // binary_data