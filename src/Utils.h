#pragma once

#include <string>

struct BinaryData;

int prepareMemFd(const BinaryData& binaryData);
std::string prepareMemPath(int fd);