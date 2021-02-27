#include "LibraryObject.h"
#include "BinaryData.h"
#include "Utils.h"

#include <unistd.h>
#include <dlfcn.h>

#include <cassert>

#include <iostream>

namespace
{

void* dlOpen(int fd)
{
    auto path = prepareMemPath(fd);
    auto* lib = dlopen(path.c_str(), RTLD_NOW);
    if(!lib)
    {
        throw std::logic_error("Lib did not open");
    }

    std::cout << "Load library" << std::endl;

    return lib;
}

Execute findExecFunc(void* libraryPtr)
{
    assert(libraryPtr);
    auto* funcPtr = dlsym(libraryPtr, "execute");
    if(!funcPtr)
    {
        throw std::logic_error("Function did not find");
    }

    return reinterpret_cast<Execute>(funcPtr);
}

} // anonymous

LibraryObject::LibraryObject(std::unique_ptr<BinaryData> binaryData)
    : m_fd(prepareMemFd(*binaryData)),
    m_libraryPtr(dlOpen(m_fd)),
    m_execFunc(findExecFunc(m_libraryPtr))
{
    if(m_execFunc)
    {
      std::cout << "Find function" << std::endl;
    }
}

LibraryObject::~LibraryObject()
{
    dlclose(m_libraryPtr);
    close(m_fd);
}

void LibraryObject::execute(const CallData& callData)
{
    assert(m_execFunc);
    m_execFunc((void*)&callData);
}