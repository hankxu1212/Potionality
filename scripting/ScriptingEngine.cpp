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

}

void ScriptingEngine::OnUpdate()
{
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
    m_Scripts[script->getClassName()] = script;
}

Behaviour* ScriptingEngine::GetScript(const std::string& scriptName)
{
    if (!Exists(scriptName)){
        return nullptr;
    }

    return m_Scripts[scriptName];
}

bool ScriptingEngine::Exists(const std::string& scriptName)
{
    return m_Scripts.find(scriptName) != m_Scripts.end();
}

void ScriptingEngine::Remove(const std::string& scriptName)
{
    if (!Exists(scriptName))
        return;
    m_Scripts.erase(scriptName);
}
