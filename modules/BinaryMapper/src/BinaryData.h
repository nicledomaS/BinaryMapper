#pragma once

#include <string>
#include <vector>

namespace binary_data
{

enum class BinaryType
{
    Unknown,
    Executable,
    Library,
    Module
};

struct BinaryData
{
    std::string name;
    BinaryType type;
    std::vector<unsigned char> data;
};

std::string toString(BinaryType type);
BinaryType fromString(const std::string& value);

} //binary_data