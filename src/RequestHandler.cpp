#include "RequestHandler.h"

#include "BinaryMapper.h"

#include "cpprest/http_msg.h"
#include "cpprest/json.h"

RequestHandler::RequestHandler()
    : m_binaryMapper(createMapper())
{
}

RequestHandler::~RequestHandler()
{
}

void RequestHandler::getHandler(web::http::http_request request)
{
    pplx::extensibility::scoped_critical_section_t lck(m_respLock);
    auto names = m_binaryMapper->getNames();
    std::vector<web::json::value> jValues(names.begin(), names.end());
    
    auto answer = web::json::value::object();
    answer["Names"] = web::json::value::array(std::move(jValues));
    request.reply(web::http::status_codes::OK, answer);
}

void RequestHandler::putHandler(web::http::http_request request)
{

}

void RequestHandler::postHandler(web::http::http_request request)
{

}

void RequestHandler::deleteHandler(web::http::http_request request)
{

}