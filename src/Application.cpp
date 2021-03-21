#include "Application.h"
#include "RequestHandler.h"
#include "HttpServer.h"

#include <thread>
#include <chrono>
#include <iostream>
    
Application::Application(const std::string& address, unsigned short port)
    : m_requestHandler(std::make_shared<RequestHandler>()),
    m_httpServer(std::make_unique<HttpServer>(m_requestHandler, address, port))
{
}

Application::~Application()
{
}

void Application::run()
{
    m_httpServer->run();

    std::cout << "Start application" << std::endl;

    using namespace std::chrono_literals;
    while(true)
    {
        std::this_thread::sleep_for(100ms);
    }
}
