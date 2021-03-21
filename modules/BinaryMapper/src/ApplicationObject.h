#pragma once

#include "BinaryObjectImpl.h"

#include <functional>
#include <string>
#include <memory>
#include <map>

struct BinaryData;
enum class ExecType;

using Strategy = std::function<std::string(const std::string&, const CallData& callData)>;

class ApplicationObject : public BinaryObjectImpl
{
public:
    ApplicationObject(std::unique_ptr<BinaryData> binaryData);
    ~ApplicationObject();

    void execute(const CallData& callData) override;

private:
    int m_fd;
    std::string m_path;
    std::map<ExecType, Strategy> m_strategies;
};
