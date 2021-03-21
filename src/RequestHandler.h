#pragma once

#include "pplx/pplx.h"

namespace web
{

namespace http
{

class http_request;

} // http

} // web

class BinaryMapper;

class RequestHandler
{
public:
    RequestHandler();
    ~RequestHandler();

    void getHandler(web::http::http_request request);
    void putHandler(web::http::http_request request);
    void postHandler(web::http::http_request request);
    void deleteHandler(web::http::http_request request);

private:
    std::unique_ptr<BinaryMapper> m_binaryMapper;
    pplx::extensibility::critical_section_t m_respLock;
};