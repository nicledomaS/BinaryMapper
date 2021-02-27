#pragma once

#include "BinaryObjectImpl.h"

#include <string>
#include <memory>

struct BinaryData;

class ApplicationObject : public BinaryObjectImpl
{
public:
    ApplicationObject(std::unique_ptr<BinaryData> binaryData);
    ~ApplicationObject();

    void execute(const CallData& callData) override;

private:
    int m_fd;
    std::string m_path;
};
