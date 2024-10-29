#include "Potion.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "../PlayMode.hpp"

void Potion::Awake()
{
    Potion::Instance = this;
}

void Potion::Start()
{
}

void Potion::Update()
{
}

void Potion::Shutdown()
{
}

template<>
void Scene::OnComponentAdded<Potion>(Entity& entity, Potion& component)
{
	this->OnComponentAdded<Behaviour>(entity, component);
}

template<>
void Scene::OnComponentRemoved<Potion>(Entity& entity, Potion& component)
{
	this->OnComponentRemoved<Behaviour>(entity, component);
}
