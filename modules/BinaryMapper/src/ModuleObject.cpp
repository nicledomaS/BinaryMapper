#include "ModuleObject.h"
#include "BinaryData.h"
#include "Utils.h"

#include <cassert>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

namespace
{

constexpr auto InsModApp = "insmod";
constexpr auto RmModApp = "rmmod";

} // anonymous

ModuleObject::ModuleObject(std::unique_ptr<binary_data::BinaryData> binaryData)
    : m_fd(prepareMemFd(*binaryData)),
    m_path(prepareMemPath(m_fd))
{
    assert(m_fd);
    auto command = std::string(InsModApp) + " " + m_path;
    system(command.c_str());
}

ModuleObject::~ModuleObject()
{
    auto command = std::string(RmModApp) + " " + m_path;
    system(command.c_str());
    close(m_fd);
}

void ModuleObject::execute(const CallData& callData)
{
}