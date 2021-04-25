#pragma once

#include <string>

namespace binary_data
{

struct BinaryData;

} // binary_data

int prepareMemFd(const binary_data::BinaryData& binaryData);
std::string prepareMemPath(int fd);