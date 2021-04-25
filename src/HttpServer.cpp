#include "HttpServer.h"
#include "RequestHandler.h"

#include "cpprest/http_listener.h"

#include <iostream>
#include <string>

namespace
{

web::http::uri createUri(std::string address, unsigned short port, std::string path)
{
    auto uriStr = std::move(address);
    uriStr.append(":" + std::to_string(port));
    uriStr.append("/" + path);

    std::cout << "createUri: " << uriStr << std::endl;

    return web::http::uri_builder(uriStr).to_uri();
}

web::http::uri createUri(std::string address, std::string path)
{
    auto uriStr = std::move(address);
    uriStr.append("/" + path);

    std::cout << "createUri: " << uriStr << std::endl;

    return web::http::uri_builder(uriStr).to_uri();
}

} // anonymous

HttpServer::HttpServer(std::string address)
    : m_listener(std::make_unique<HttpListener>(createUri(std::move(address), "")))
{
}

HttpServer::HttpServer(std::string address, unsigned short port)
    : m_listener(std::make_unique<HttpListener>(createUri(std::move(address), port, "")))
{
}

HttpServer::~HttpServer()
{
    m_listener->close().wait();
}

void HttpServer::run()
{
    for(const auto& pair : m_requestHandlers)
    {
        m_listener->support(pair.first, pair.second);
    }

    m_listener->open().wait();

    std::cout << "Start http server" << std::endl;
}

void HttpServer::addRequestHandler(web::http::method method, Handler requestHandler)
{
    m_requestHandlers.insert({ std::move(method), std::move(requestHandler) });
}
