#pragma once

#include "BinaryObjectImpl.h"

#include <vector>
#include <memory>

namespace binary_data
{

struct BinaryData;

} // binary_data

class ModuleObject: public BinaryObjectImpl
{
public:
    ModuleObject(std::unique_ptr<binary_data::BinaryData> binaryData);
    ~ModuleObject();
    
    void execute(const CallData& callData) override;

private:
    int m_fd;
    std::string m_path;
};