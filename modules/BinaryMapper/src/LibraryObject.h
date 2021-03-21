#pragma once

#include "BinaryObjectImpl.h"

#include <vector>
#include <memory>

namespace binary_data
{

struct BinaryData;

} // binary_data

using Execute = void(*)(void*);

class LibraryObject: public BinaryObjectImpl
{
public:
    LibraryObject(std::unique_ptr<binary_data::BinaryData> binaryData);
    ~LibraryObject();
    
    void execute(const CallData& callData) override;

private:
    int m_fd;
    void* m_libraryPtr;
    Execute m_execFunc;
};