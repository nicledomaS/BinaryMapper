#pragma once

#include "BinaryObjectImpl.h"

#include <vector>
#include <memory>

struct BinaryData;

class ModuleObject: public BinaryObjectImpl
{
public:
    ModuleObject(std::unique_ptr<BinaryData> binaryData);
    ~ModuleObject();
    
    void execute(const CallData& callData) override;

private:
    int m_fd;
    std::string m_path;
};