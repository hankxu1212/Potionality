#include "Customer.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "../PlayMode.hpp"

void Customer::Awake()
{
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
		LOG_INFO("Customer is close! Render some text here");
		// Note: Customer is huge, interaction distance seems to be around its top left corner
		// RenderText("WHATS UP", -.90f,-0.8f, .003f, characters,font_texs);
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