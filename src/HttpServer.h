#pragma once

#include "cpprest/http_msg.h"

#include <memory>
#include <map>

namespace web
{

namespace http
{

namespace experimental
{

namespace listener
{

class http_listener;

} // listener

} // experimental

} // http

} // web

using HttpListener = web::http::experimental::listener::http_listener;
using Handler = std::function<void(web::http::http_request)>;

class RequestHandler;

class HttpServer
{
public:
    explicit HttpServer(std::string address);
    HttpServer(std::string address, unsigned short port);
    ~HttpServer();

    void run();

    void addRequestHandler(web::http::method method, Handler requestHandler);

private:
    std::unique_ptr<HttpListener> m_listener;
    std::map<web::http::method, Handler> m_requestHandlers;
};