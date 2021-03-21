#pragma once

#include <memory>

class RequestHandler;
class HttpServer;

class Application
{
public:
    Application(const std::string& address, unsigned short port);
    ~Application();

    void run();

private:
    std::shared_ptr<RequestHandler> m_requestHandler;
    std::unique_ptr<HttpServer> m_httpServer;
};