#include "Application.h"
#include "RequestHandler.h"
#include "HttpServer.h"

#include "cpprest/http_msg.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <functional>

using namespace std::placeholders;
    
Application::Application(const std::string& address, unsigned short port)
    : m_requestHandler(std::make_shared<RequestHandler>()),
    m_httpServer(std::make_unique<HttpServer>(address, port))
{
}

Application::~Application()
{
}

void Application::run()
{
    m_httpServer->addRequestHandler(
        web::http::methods::GET, std::bind(&RequestHandler::getHandler, m_requestHandler, _1));
    m_httpServer->addRequestHandler(
        web::http::methods::PUT, std::bind(&RequestHandler::putHandler, m_requestHandler, _1));
    m_httpServer->addRequestHandler(
        web::http::methods::POST, std::bind(&RequestHandler::postHandler, m_requestHandler, _1));
    m_httpServer->addRequestHandler(
        web::http::methods::DEL, std::bind(&RequestHandler::deleteHandler, m_requestHandler, _1));
    
    m_httpServer->run();

    std::cout << "Start application" << std::endl;

    using namespace std::chrono_literals;
    while(true)
    {
        std::this_thread::sleep_for(100ms);
    }
}
