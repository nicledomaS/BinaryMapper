#pragma once

#include <string>

namespace call_data
{

enum class ExecType
{
    Unknown,
    LocalExec,
    LoadExec
};

std::string toString(ExecType type);
ExecType fromString(const std::string& value);

} // call_data