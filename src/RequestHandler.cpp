#include "RequestHandler.h"

#include "BinaryMapper.h"
#include "BinaryObject.h"
#include "BinaryData.h"
#include "CallData.h"
#include "ExecType.h"

#include "cpprest/http_msg.h"
#include "cpprest/json.h"

#include <iostream>
#include <functional>
#include <map>

namespace
{

constexpr auto NameKey = "name";
constexpr auto TypeKey = "type";
constexpr auto BinaryDataKey = "binary_data";

constexpr auto NamesKey = "Names";
constexpr auto Factories = "Factories";
constexpr auto ErrorTag = "ErrorTag";
constexpr auto ErrorMessage = "ErrorMessage";

constexpr auto UriNamesPath = "/binary/names";
constexpr auto UriFactoriesPath = "/binary/factories";
constexpr auto UriAddBinaryDataPath = "/binary/AddBinaryData";
constexpr auto UriDoCallPath = "/binary/DoCall";

Handler findHandler(const std::map<std::string, Handler>& handlers, const std::string& path)
{
    auto it = handlers.find(path);
    return it == handlers.end() ? Handler() : it->second;
}

void handle(const std::map<std::string, Handler>& handlers, web::http::http_request request)
{
    if(auto handler = findHandler(handlers, request.request_uri().path()))
    {
        handler(request);
    }
    else
    {
        request.reply(web::http::status_codes::NotFound);
    }
}

web::json::value prepareError(const std::string& tag, const std::string& message)
{
    auto error = web::json::value::object();
    error[ErrorTag] = web::json::value::string(tag);
    error[ErrorMessage] = web::json::value::string(message);

    return error;
}

std::unique_ptr<binary_data::BinaryData> toBinaryData(const web::json::value& value)
{
    if(value.is_object())
    {
        auto obj = value.as_object();
        auto binaryData = std::make_unique<binary_data::BinaryData>();
        binaryData->name = obj[NameKey].as_string();
        binaryData->type = binary_data::fromString(obj[TypeKey].as_string());
        binaryData->data = utility::conversions::from_base64(obj[BinaryDataKey].as_string());

        return binaryData;
    }
    
    return nullptr;
}

void setString(CallData& callData, const std::string& name, const web::json::value& value)
{
    callData.setValue(name, value.as_string());
}

void setBool(CallData& callData, const std::string& name, const web::json::value& value)
{
    callData.setValue(name, value.as_bool());
}

void setNumber(CallData& callData, const std::string& name, const web::json::value& value)
{
    if(value.is_integer())
    {
        callData.setValue(name, value.as_integer());
    }
    else if(value.is_double())
    {
        callData.setValue(name, value.as_double());
    }
    else
    {
        std::cout << "Number do not support" << std::endl;
    }
}

using ValueHandler = std::function<void(CallData&, const std::string&, const web::json::value&)>;
static std::map<web::json::value::value_type, ValueHandler> valueHandlers = {
    { web::json::value::value_type::String, &setString },
    { web::json::value::value_type::Boolean, &setBool },
    { web::json::value::value_type::Number, &setNumber }
};       

std::unique_ptr<CallData> toCallData(const web::json::value& value)
{
    if(value.is_object())
    {
        auto obj = value.as_object();
        auto callData = std::make_unique<CallData>();

        for(const auto& item : obj)
        {
            auto it = valueHandlers.find(item.second.type());
            if(it != valueHandlers.end())
            {
                it->second(*callData, item.first, item.second);
            }
            else
            {
                std::cout << "Type do not support" << std::endl;
            }
        }

        return callData;
    }
    
    return nullptr;
}

} // anonymous

RequestHandler::RequestHandler()
    : m_binaryMapper(createMapper())
{
    m_getHandlers.insert({ UriNamesPath, [&](web::http::http_request request)
    {
        auto names = getBinaryNames();
        std::vector<web::json::value> jValues(names.begin(), names.end());

        auto answer = web::json::value::object();
        answer[NamesKey] = web::json::value::array(std::move(jValues));
        request.reply(web::http::status_codes::OK, answer);
    } });

    m_getHandlers.insert({ UriFactoriesPath, [&](web::http::http_request request)
    {
        auto names = getFactoryNames();
        std::vector<web::json::value> jValues(names.begin(), names.end());

        auto answer = web::json::value::object();
        answer[Factories] = web::json::value::array(std::move(jValues));
        request.reply(web::http::status_codes::OK, answer);
    } });

    m_postHandlers.insert({ UriAddBinaryDataPath, [&](web::http::http_request request)
    {
        web::json::value error;
        try
        {
            auto obj = request.extract_json().get();
            if(auto binData = toBinaryData(obj))
            {
                addBinaryData(std::move(binData));
                request.reply(web::http::status_codes::OK);
                return;
            }

            error = prepareError(UriAddBinaryDataPath, "Bad body");
        }
        catch(const std::exception& ex)
        {
            error = prepareError(UriAddBinaryDataPath, ex.what());
        }

        request.reply(web::http::status_codes::BadRequest, error);
    } });

    m_postHandlers.insert({ UriDoCallPath, [&](web::http::http_request request)
    {
        web::json::value error;
        try
        {
            auto obj = request.extract_json().get();
            if(auto callData = toCallData(obj))
            {
                doCall(std::move(callData));
                request.reply(web::http::status_codes::OK);
                return;
            }

            error = prepareError(UriAddBinaryDataPath, "Bad body");
        }
        catch(const std::exception& ex)
        {
            error = prepareError(UriAddBinaryDataPath, ex.what());
        }

        request.reply(web::http::status_codes::BadRequest, error);
    } });

    m_deleteHandlers.insert({ UriNamesPath, [&](web::http::http_request request)
    {
        web::json::value error;
        try
        {
            auto value = request.extract_json().get();

            if(value.is_object())
            {
                auto obj = value.as_object();
                auto arrayNames = obj[NamesKey].as_array();
                
                if(arrayNames.size())
                {
                    std::vector<std::string> names;
                    std::transform(
                        arrayNames.begin(),
                        arrayNames.end(),
                        std::back_inserter(names),
                        [](const auto& item) { return item.as_string(); });
                    deleteBinaryData(names);
                    request.reply(web::http::status_codes::OK);
                    return;
                }
            }

            error = prepareError(UriNamesPath, "Bad body");
        }
        catch(const std::exception& ex)
        {
            error = prepareError(UriNamesPath, ex.what());
        }

        request.reply(web::http::status_codes::BadRequest, error);
    } });
}

RequestHandler::~RequestHandler()
{
}

void RequestHandler::getHandler(web::http::http_request request)
{
    handle(m_getHandlers, request);
}

void RequestHandler::putHandler(web::http::http_request request)
{
    handle(m_putHandlers, request);
}

void RequestHandler::postHandler(web::http::http_request request)
{
    handle(m_postHandlers, request);
}

void RequestHandler::deleteHandler(web::http::http_request request)
{
    handle(m_deleteHandlers, request);
}

std::vector<std::string> RequestHandler::getBinaryNames() const
{
    pplx::extensibility::scoped_critical_section_t lck(m_respLock);
    return m_binaryMapper->getBinaryNames();
}

std::vector<std::string> RequestHandler::getFactoryNames() const
{
    pplx::extensibility::scoped_critical_section_t lck(m_respLock);
    return m_binaryMapper->getFactoryNames();
}

bool RequestHandler::addBinaryData(std::unique_ptr<binary_data::BinaryData> binaryData)
{
    pplx::extensibility::scoped_critical_section_t lck(m_respLock);
    auto obj = m_binaryMapper->create(std::move(binaryData));

    return obj != nullptr;
}

void RequestHandler::deleteBinaryData(const std::vector<std::string>& names)
{
    pplx::extensibility::scoped_critical_section_t lck(m_respLock);
    for(const auto& name : names)
    {
        m_binaryMapper->destroy(name);
    }
}

void RequestHandler::doCall(std::unique_ptr<CallData> callData)
{
    pplx::extensibility::scoped_critical_section_t lck(m_respLock);
    if(auto binaryData = m_binaryMapper->find(callData->getValue<std::string>(NameKey)))
    {
        binaryData->execute(*callData);
    }
}