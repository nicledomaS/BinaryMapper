#include <iostream>
#include <thread>
#include <chrono>

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "BinaryMapper.h"
#include "BinaryData.h"
#include "BinaryObject.h"
#include "CallData.h"
#include "ExecType.h"

std::unique_ptr<binary_data::BinaryData> LoadBinaryData(const std::string& path, const std::string& name, binary_data::BinaryType type)
{
  auto binaryData = std::make_unique<binary_data::BinaryData>();
  binaryData->name = name;
  binaryData->type = type;

  auto fd = open(path.c_str(), O_RDONLY);
  std::vector<unsigned char> buf(1024);

  while(auto size = read(fd, buf.data(), buf.size()))
  {
    binaryData->data.insert(binaryData->data.end(), buf.begin(), buf.begin() + size);
  }

  close(fd);

  return binaryData;
}

int main(int argc, const char* argv[])
{
  std::cout << "Start" << std::endl;

  std::cout << getpid() << std::endl;

  auto mapper = createMapper();

  auto libBinData = LoadBinaryData(
    "build/examples/test_export/libtest_export.so",
    "libtest_export.so",
    binary_data::BinaryType::Library);
  auto libWrapper = mapper->create(std::move(libBinData));

  auto execBinData = LoadBinaryData(
    "build/examples/test_exec/test_exec",
    "test_exec",
    binary_data::BinaryType::Executable);
  auto execWrapper = mapper->create(std::move(execBinData));

  auto shExecBinData = LoadBinaryData(
    "build/examples/sh_example/sh_example.sh",
    "sh_example.sh",
    binary_data::BinaryType::Executable);
  auto shExecWrapper = mapper->create(std::move(shExecBinData));

  auto pyExecBinData = LoadBinaryData(
    "build/examples/py_example/py_example.py",
    "py_example.py",
    binary_data::BinaryType::Executable);
  auto pyExecWrapper = mapper->create(std::move(pyExecBinData));

  // auto moduleBinData = LoadBinaryData(
  //   "build/examples/driver_example/driver_example.ko",
  //   "driver_example.ko",
  //   BinaryType::Module);
  // auto moduleWrapper = mapper->create(std::move(moduleBinData));

  CallData libCallData;
  libCallData.setValue<std::string>("args", "test");

  CallData shExecCallData;
  shExecCallData.setValue<call_data::ExecType>("type", call_data::ExecType::LocalExec);
  shExecCallData.setValue<std::string>("command", "sh");
  shExecCallData.setValue<std::string>("args", "test");

  CallData puExecCallData;
  puExecCallData.setValue<call_data::ExecType>("type", call_data::ExecType::LocalExec);
  puExecCallData.setValue<std::string>("command", "python3");
  puExecCallData.setValue<std::string>("args", "test");

  CallData loadExecCallData;
  loadExecCallData.setValue<call_data::ExecType>("type", call_data::ExecType::LoadExec);
  loadExecCallData.setValue<std::string>("args", "test");
  

  while(true)
  {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    libWrapper->execute(libCallData);
    execWrapper->execute(loadExecCallData);
    shExecWrapper->execute(shExecCallData);
    pyExecWrapper->execute(puExecCallData);
    // moduleWrapper->execute(libCallData);
  }

  return 0;
}
