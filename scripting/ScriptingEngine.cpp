#include "ScriptingEngine.hpp"
#include "Behaviour.hpp"
#include <utility>

ScriptingEngine* ScriptingEngine::s_Instance = nullptr;

ScriptingEngine::ScriptingEngine() : Layer("ScriptingEngine") {
    s_Instance = this;
}

void ScriptingEngine::OnAttach()
{
    for (const auto& script : m_Scripts)
    {
        script.second->Start();
    }
}

void ScriptingEngine::OnDetach()
{
    for (const auto& script : m_Scripts)
    {
        script.second->Shutdown();
    }
}

void ScriptingEngine::OnUpdate()
{
    // std::cout << m_Scripts.size() << " scripts found\n";
}

bool ScriptingEngine::OnEvent(const SDL_Event& e)
{
    for (auto it = m_Scripts.begin(); it != m_Scripts.end(); it++) {
        if(it->second->HandleEvent(e))
            return true;
    }
    return false;
}
 
void ScriptingEngine::Add(Behaviour* script)
{
    m_Scripts[std::make_pair(script->getClassName(), (size_t)script->GetEntityID())] = script;
}