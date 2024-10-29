#include "Customer.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "../PlayMode.hpp"

void Customer::Awake()
{
	Customer::Instance = this;
}

void Customer::Start()
{
}

void Customer::Update()
{
	auto& playerPos = Player::Instance->GetTransform()->position;
	float distance = glm::distance(GetTransform()->position, playerPos);

	if (distance < interactionDistance)
	{
		RenderText("Deliver!", GetTransform()->position.x, 1080 - GetTransform()->position.y);
	}
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