#include <iostream>
#include <string>

#include "CallData.h"

void printCallData(void* data)
{
  auto callData = (CallData*)data;
  if(callData->size())
  {
      std::cout << "test_export" << std::endl;
      std::cout << callData->getValue<std::string>("args") << std::endl;
  }
}

extern "C" void execute(void* data)
{
  printCallData(data);
}