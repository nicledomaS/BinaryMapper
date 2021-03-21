#pragma once

struct CallData;

class BinaryObjectImpl
{
public:
    virtual ~BinaryObjectImpl() = default;
    virtual void execute(const CallData& callData) = 0;
};