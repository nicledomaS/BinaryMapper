#pragma once

#include "pplx/pplx.h"

#include <map>

namespace web
{

namespace http
{

class http_request;

} // http

} // web

class BinaryMapper;

namespace binary_data
{

class BinaryData;

} // binary_data

class CallData;

using Handler = std::function<void(web::http::http_request)>;

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
    std::vector<std::string> getBinaryNames() const;
    std::vector<std::string> getFactoryNames() const;
    bool addBinaryData(std::unique_ptr<binary_data::BinaryData> binaryData);
    void deleteBinaryData(const std::vector<std::string>& names);
    void doCall(std::unique_ptr<CallData> callData);

    std::unique_ptr<BinaryMapper> m_binaryMapper;
    mutable pplx::extensibility::critical_section_t m_respLock;
    std::map<std::string, Handler> m_getHandlers;
    std::map<std::string, Handler> m_putHandlers;
    std::map<std::string, Handler> m_postHandlers;
    std::map<std::string, Handler> m_deleteHandlers;
};