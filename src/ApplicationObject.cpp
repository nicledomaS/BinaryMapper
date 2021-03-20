#include "ApplicationObject.h"
#include "BinaryData.h"
#include "CallData.h"
#include "Utils.h"

#include <stdlib.h>
#include <unistd.h>

ApplicationObject::ApplicationObject(std::unique_ptr<BinaryData> binaryData)
    : m_fd(prepareMemFd(*binaryData)),
    m_path(prepareMemPath(m_fd))
{
}

ApplicationObject::~ApplicationObject()
{
    close(m_fd);
}

void ApplicationObject::execute(const CallData& callData)
{
    auto callStr = callData.size() == 0 ?
        m_path : m_path + " " +  callData.getValue<std::string>("args");

    system(callStr.c_str());
}