#pragma once

#include <string>
#include <vector>

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