#include "ExecType.h"

#include <algorithm>
#include <map>

namespace call_data
{

namespace
{

constexpr auto UnknownStr = "Unknown";
constexpr auto LocalExecStr = "LocalExec";
constexpr auto LoadExecStr = "LoadExec";

static std::map<std::string, ExecType> execTypes = {
    { LocalExecStr, ExecType::LocalExec },
    { LoadExecStr, ExecType::LoadExec }
};

} // anonymous

std::string toString(ExecType type)
{
    auto it = find_if(execTypes.begin(), execTypes.end(), [type](const auto& pair)
    {
        return pair.second == type;
    });

    return it == execTypes.end() ? UnknownStr : it->first;
}

ExecType fromString(const std::string& value)
{
    auto it = execTypes.find(value);
    return it == execTypes.end() ? ExecType::Unknown : it->second;
}

} // call_data