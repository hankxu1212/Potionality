#include "Behaviour.hpp"

#include "../scene/Scene.hpp"
#include "ScriptingEngine.hpp"

template<> 
void Scene::OnComponentAdded<Behaviour>(Entity& entity, Behaviour& component)
{
	component.Awake();
	ScriptingEngine::Get()->Add(&component);
}

template<>
void Scene::OnComponentRemoved<Behaviour>(Entity& entity, Behaviour& component)
{
	component.Shutdown();
	ScriptingEngine::Get()->Remove(&component);
}