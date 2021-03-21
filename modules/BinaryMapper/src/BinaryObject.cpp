#include "BinaryObject.h"
#include "BinaryObjectImpl.h"

BinaryObject::BinaryObject(std::unique_ptr<BinaryObjectImpl> impl)
    : m_impl(std::move(impl))
{
}

void BinaryObject::execute(const CallData& callData)
{
    m_impl->execute(callData);
}