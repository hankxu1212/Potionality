#pragma once

#include "../core/layers/Layer.hpp"
#include "../core/utils/Singleton.hpp"
#include "../scene/UUID.hpp"

#include <unordered_map>

class Behaviour;

class ScriptingEngine : public Layer, Singleton
{
private:
    static ScriptingEngine* s_Instance;

public:
    ScriptingEngine();

public:
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    bool OnEvent(const SDL_Event& event) override;

public:
    static ScriptingEngine* Get() { return s_Instance; }

    void Add(Behaviour*);

    void Remove(Behaviour*);
    
private:
    struct string_size_t_hash {
        std::size_t operator()(const std::pair<std::string, size_t>& p) const {
            std::size_t h1 = std::hash<std::string>{}(p.first);
            std::size_t h2 = std::hash<size_t>{}(p.second);

            // Combine the two hash values with XOR and a left shift
            return h1 ^ (h2 << 1); 
        }
    };

    std::unordered_map<std::pair<std::string, size_t>, Behaviour*, string_size_t_hash> m_Scripts;
};
