#pragma once

#include "BinaryObjectImpl.h"

#include <functional>
#include <string>
#include <memory>
#include <map>

namespace binary_data
{

struct BinaryData;

} // binary_data

namespace call_data
{

enum class ExecType;

} // call_data

using Strategy = std::function<std::string(const std::string&, const CallData& callData)>;

class ApplicationObject : public BinaryObjectImpl
{
public:
    ApplicationObject(std::unique_ptr<binary_data::BinaryData> binaryData);
    ~ApplicationObject();

    void execute(const CallData& callData) override;

private:
    int m_fd;
    std::string m_path;
    std::map<call_data::ExecType, Strategy> m_strategies;
};
