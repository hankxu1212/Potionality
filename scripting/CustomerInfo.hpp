#pragma once

#include "PotionCore.hpp"
#include "../scene/Scene.hpp"
#include "../core/Files.hpp"

struct CustomerInfo
{
    std::string m_Type;
    PotionRequest m_Request;
    float m_Patience;
    std::vector<std::string> m_MonologueOnAsk;
    std::vector<std::string> m_MonologueOnRecieveSuccess;
    std::vector<std::string> m_MonologueOnRecieveFailure;

    void Deserialize(const Scene::TValueMap& customer)
    {
        m_Request.potionsToMake.clear();
        m_MonologueOnAsk.clear();
        m_MonologueOnRecieveSuccess.clear();
        m_MonologueOnRecieveFailure.clear();

        m_Type = customer.at("type").as_string().value();
        const auto& request = customer.at("request").as_object().value();
        m_Request.reward = request.at("reward").as_float();

        const auto& potions = request.at("potions").as_array().value();
        for (const auto& potion : potions)
        {
            m_Request.potionsToMake.emplace_back(potion.as_string().value());
        }

        m_Patience = customer.at("patience").as_float();
        const auto& askArr = customer.at("ask").as_array().value();
        for (const auto& ask : askArr)
            m_MonologueOnAsk.push_back(ask.as_string().value());

        const auto& successArr = customer.at("recieve_success").as_array().value();
        for (const auto& succ : successArr)
            m_MonologueOnRecieveSuccess.push_back(succ.as_string().value());

        const auto& failureArr = customer.at("recieve_failure").as_array().value();
        for (const auto& fail : failureArr)
            m_MonologueOnRecieveFailure.push_back(fail.as_string().value());

        // add debug messaging
        LOG_DEBUG("Displaying customer info loaded: ", Logger::CYAN, Logger::BOLD);
        std::cout << "Loaded customer: " << m_Type;
        std::cout << " with request: ";
        for (const auto& p : m_Request.potionsToMake){
            std::cout << p << " ";
        }
        std::cout << "\n with ask: ";
        for (const auto& p : m_MonologueOnAsk){
            std::cout << p << " ";
        }
        std::cout << "\n with success: ";
        for (const auto& p : m_MonologueOnRecieveSuccess){
            std::cout << p << " ";
        }
        std::cout << "\n with fails: ";
        for (const auto& p : m_MonologueOnRecieveFailure){
            std::cout << p << " ";
        }
        std::cout << "\n with patience: " << m_Patience;
        std::cout << std::endl;
    }
};