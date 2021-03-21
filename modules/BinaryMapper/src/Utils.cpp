#include "Utils.h"
#include "BinaryData.h"

#include <sys/mman.h>
#include <unistd.h>

int prepareMemFd(const binary_data::BinaryData& binaryData)
{
    auto fd = memfd_create(binaryData.name.c_str(), MFD_ALLOW_SEALING);
    ::write(fd, binaryData.data.data(), binaryData.data.size());

    return fd;
}

std::string prepareMemPath(int fd)
{
    return std::string("/proc/") + std::to_string(getpid()) + "/fd/" + std::to_string(fd);
}