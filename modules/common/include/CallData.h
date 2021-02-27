#pragma once

#include <string>
#include <any>
#include <map>
#include <stdexcept>

class CallData
{
public:
    template <typename T>
    void setValue(const std::string& name, const T& value)
    {
        m_data.insert({ name, value });
    }

    template <typename T>
    T getValue(const std::string& name) const
    {
        auto it = m_data.find(name);
        if(it == m_data.end())
        {
            throw std::invalid_argument(name);
        } 

        return std::any_cast<T>(it->second);
    }

    size_t size() const
    {
        return m_data.size();
    }
private:
    std::map<std::string, std::any> m_data;
};