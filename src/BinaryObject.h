#pragma once

#include <memory>

class BinaryObjectImpl;
struct CallData;

class BinaryObject
{
public:
    BinaryObject(std::unique_ptr<BinaryObjectImpl> impl);
    void execute(const CallData& callData);

private:
    std::unique_ptr<BinaryObjectImpl> m_impl;
};