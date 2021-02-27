#include "ApplicationObject.h"
#include "BinaryData.h"
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
    system(m_path.c_str());
}