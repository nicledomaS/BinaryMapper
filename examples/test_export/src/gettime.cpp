#include <iostream>
#include <string>

#include "CallData.h"

void printCallData(const std::string& strValue, int intValue)
{
      std::cout << strValue << std::to_string(intValue) << std::endl;
}

void printCallData(void* data)
{
  auto callData = static_cast<CallData*>(data);
  if(callData->size())
  {
    std::cout << "test_export" << std::endl;

    auto command = callData->getValue<std::string>("command");
    if(command == "printCallData")
    {
      printCallData(callData->getValue<std::string>("str_value"), callData->getValue<int>("int_value"));
    }
  }
}

extern "C" void execute(void* data)
{
  printCallData(data);
}