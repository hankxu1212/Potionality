#include "Customer.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"

void Customer::Awake()
{
}

void Customer::Start()
{
}

void Customer::Update()
{
}

void Customer::Shutdown()
{
}

template<>
void Scene::OnComponentAdded<Customer>(Entity& entity, Customer& component)
{
	this->OnComponentAdded<Behaviour>(entity, component);
}

template<>
void Scene::OnComponentRemoved<Customer>(Entity& entity, Customer& component)
{
	this->OnComponentRemoved<Behaviour>(entity, component);
}