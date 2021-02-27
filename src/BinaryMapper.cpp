#include "BinaryMapper.h"
#include "BinaryData.h"

#include <iostream>
#include <cassert>

std::shared_ptr<BinaryObject> BinaryMapper::create(std::unique_ptr<BinaryData> binaryData)
{
    auto it = m_factories.find(binaryData->type);
    assert((it != m_factories.end()));

    auto name = binaryData->name;
    auto wrapper = it->second(std::move(binaryData));
    assert(wrapper);
    m_wrappers.insert({ name, wrapper });

    return wrapper;
}

void BinaryMapper::destroy(const std::string& name)
{
    auto it = m_wrappers.find(name);
    if(it == m_wrappers.end())
    {
        // TODO: used log library
        std::cout << "Binary wrapper did not found!" << std::endl;
    }
    else
    {
        m_wrappers.erase(it);
    }
}

std::shared_ptr<BinaryObject> BinaryMapper::find(const std::string& name)
{
    auto it = m_wrappers.find(name);
    return it == m_wrappers.end() ? nullptr : it->second;
}

void BinaryMapper::addFactory(BinaryType type, BinaryObjectFactory factory)
{
    m_factories.insert({ type, factory });
}