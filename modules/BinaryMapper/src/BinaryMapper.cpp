#include "BinaryMapper.h"
#include "BinaryData.h"
#include "BinaryObject.h"
#include "LibraryObject.h"
#include "ApplicationObject.h"
#include "ModuleObject.h"

#include <iostream>
#include <cassert>

std::unique_ptr<BinaryMapper> createMapper()
{
    auto mapper = std::make_unique<BinaryMapper>();
    mapper->addFactory(binary_data::BinaryType::Library, [](std::unique_ptr<binary_data::BinaryData> binaryData)
    {
        return std::make_shared<BinaryObject>(std::make_unique<LibraryObject>(std::move(binaryData)));
    });
    mapper->addFactory(binary_data::BinaryType::Executable, [](std::unique_ptr<binary_data::BinaryData> binaryData)
    {
        return std::make_shared<BinaryObject>(std::make_unique<ApplicationObject>(std::move(binaryData)));
    });
    mapper->addFactory(binary_data::BinaryType::Module, [](std::unique_ptr<binary_data::BinaryData> binaryData)
    {
        return std::make_shared<BinaryObject>(std::make_unique<ModuleObject>(std::move(binaryData)));
    });

    return mapper;
}

std::shared_ptr<BinaryObject> BinaryMapper::create(std::unique_ptr<binary_data::BinaryData> binaryData)
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

void BinaryMapper::addFactory(binary_data::BinaryType type, BinaryObjectFactory factory)
{
    m_factories.insert({ type, factory });
}

std::vector<std::string> BinaryMapper::getBinaryNames() const
{
    std::vector<std::string> names;
    std::transform(
        m_wrappers.begin(),
        m_wrappers.end(),
        std::back_inserter(names),
        [](const auto& pair)
        {
            return pair.first;
        });

    return names;
}

std::vector<std::string> BinaryMapper::getFactoryNames() const
{
    std::vector<std::string> names;
    std::transform(
        m_factories.begin(),
        m_factories.end(),
        std::back_inserter(names),
        [](const auto& pair)
        {
            return toString(pair.first);
        });

    return names;
}