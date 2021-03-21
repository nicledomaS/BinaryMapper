#pragma once

#include <memory>

namespace web
{

namespace http
{

class http_request;

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

class RequestHandler;

class HttpServer
{
public:
    HttpServer(
        std::shared_ptr<RequestHandler> requestHandler,
        const std::string& address);
    HttpServer(
        std::shared_ptr<RequestHandler> requestHandler,
        const std::string& address,
        unsigned short port);
    ~HttpServer();

    void run();

    void getHandler(web::http::http_request request);
    void putHandler(web::http::http_request request);
    void postHandler(web::http::http_request request);
    void deleteHandler(web::http::http_request request);

private:
    std::shared_ptr<RequestHandler> m_requestHandler;
    std::unique_ptr<HttpListener> m_listener;
};