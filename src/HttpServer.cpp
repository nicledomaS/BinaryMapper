#include "HttpServer.h"
#include "RequestHandler.h"

#include "cpprest/http_listener.h"

#include <iostream>
#include <string>

web::http::uri createUri(const std::string& address, unsigned short port, const std::string& path)
{
    auto uriStr = address;
    uriStr.append(":" + std::to_string(port));
    uriStr.append("/" + path);

    std::cout << "createUri: " << uriStr << std::endl;

    return web::http::uri_builder(uriStr).to_uri();
}

web::http::uri createUri(const std::string& address, const std::string& path)
{
    auto uriStr = address;
    uriStr.append("/" + path);

    std::cout << "createUri: " << uriStr << std::endl;

    return web::http::uri_builder(uriStr).to_uri();
}

HttpServer::HttpServer(
    std::shared_ptr<RequestHandler> requestHandler,
    const std::string& address)
    : m_requestHandler(requestHandler),
    m_listener(std::make_unique<HttpListener>(createUri(address, "")))
{
}

HttpServer::HttpServer(
    std::shared_ptr<RequestHandler> requestHandler,
    const std::string& address,
    unsigned short port)
    : m_requestHandler(requestHandler),
    m_listener(std::make_unique<HttpListener>(createUri(address, port, "")))
{
}

HttpServer::~HttpServer()
{
    m_listener->close().wait();
}

void HttpServer::run()
{
    m_listener->support(
        web::http::methods::GET,
        std::bind(&HttpServer::getHandler, this, std::placeholders::_1));

    m_listener->open().wait();

    std::cout << "Start http server" << std::endl;
}

void HttpServer::getHandler(web::http::http_request request)
{
    m_requestHandler->getHandler(request);
}

void HttpServer::putHandler(web::http::http_request request)
{
    m_requestHandler->putHandler(request);
}

void HttpServer::postHandler(web::http::http_request request)
{
    m_requestHandler->postHandler(request);
}

void HttpServer::deleteHandler(web::http::http_request request)
{
    m_requestHandler->postHandler(request);
}
