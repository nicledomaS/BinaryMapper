#include <iostream>
#include <thread>
#include <chrono>

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "BinaryMapper.h"
#include "BinaryData.h"
#include "BinaryObject.h"
#include "LibraryObject.h"
#include "ApplicationObject.h"
#include "CallData.h"

std::unique_ptr<BinaryData> LoadBinaryData(const std::string& path, const std::string& name, BinaryType type)
{
  auto binaryData = std::make_unique<BinaryData>();
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

  BinaryMapper mapper;
  mapper.addFactory(BinaryType::Library, [](std::unique_ptr<BinaryData> binaryData)
  {
    return std::make_shared<BinaryObject>(std::make_unique<LibraryObject
  >(std::move(binaryData)));
  });
  mapper.addFactory(BinaryType::Executable, [](std::unique_ptr<BinaryData> binaryData)
  {
    return std::make_shared<BinaryObject>(std::make_unique<ApplicationObject>(std::move(binaryData)));
  });

  auto libBinData = LoadBinaryData(
    "build/modules/test_export/libtest_export.so",
    "libtest_export.so",
    BinaryType::Library);
  auto libWrapper = mapper.create(std::move(libBinData));

  auto execBinData = LoadBinaryData(
    "build/modules/test_exec/test_exec",
    "test_exec",
    BinaryType::Executable);
  auto execWrapper = mapper.create(std::move(execBinData));

  CallData data;
  data.setValue<std::string>("args", "test");

  while(true)
  {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    libWrapper->execute(data);
    execWrapper->execute(data);
  }

  return 0;
}
