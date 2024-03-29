#include "ApplicationObject.h"
#include "BinaryData.h"
#include "CallData.h"
#include "Utils.h"
#include "ExecType.h"

#include <cassert>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>


namespace
{

constexpr auto TypeKey = "type";
constexpr auto CommandKey = "command";
constexpr auto ArgsKey = "args";

std::string prepareLocalExecComand(const std::string& path, const CallData& callData)
{
    assert(callData.size());

    auto command = callData.getValue<std::string>(CommandKey);
    auto args = callData.getValue<std::string>(ArgsKey);
    return command + " " + path + " " + args;
}

std::string prepareLoadExecComand(const std::string& path, const CallData& callData)
{
    return callData.size() == 0 ? path : path + " " +  callData.getValue<std::string>(ArgsKey);
}

} // anonymous

ApplicationObject::ApplicationObject(std::unique_ptr<binary_data::BinaryData> binaryData)
    : m_fd(prepareMemFd(*binaryData)),
    m_path(prepareMemPath(m_fd)),
    m_strategies({
        { call_data::ExecType::LocalExec, prepareLocalExecComand },
        { call_data::ExecType::LoadExec, prepareLoadExecComand }
    })
{
}

ApplicationObject::~ApplicationObject()
{
    close(m_fd);
}

void ApplicationObject::execute(const CallData& callData)
{
    assert(callData.size());

    auto type = call_data::fromString(callData.getValue<std::string>(TypeKey));
    auto it = m_strategies.find(type);
    if(it != m_strategies.end())
    {
        auto callStr = it->second(m_path, callData);
        system(callStr.c_str());
    }
    else
    {
        std::cout << "Exec type did not found" << std::endl;
    }    
}