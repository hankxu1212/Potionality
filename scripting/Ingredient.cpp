#include "Ingredient.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "../PlayMode.hpp"

void Ingredient::Awake()
{
}

void Ingredient::Start()
{
}

void Ingredient::Update()
{
	auto& playerPos = Player::Instance->GetTransform()->position;
	float distance = glm::distance(GetTransform()->position, playerPos);

	if (distance < interactionDistance)
	{
		LOG_INFO("Smashable! Render some text here!"); // TODO: Render text
	}
}

void Ingredient::Shutdown()
{
}

template<>
void Scene::OnComponentAdded<Ingredient>(Entity& entity, Ingredient& component)
{
	this->OnComponentAdded<Behaviour>(entity, component);
}

template<>
void Scene::OnComponentRemoved<Ingredient>(Entity& entity, Ingredient& component)
{
	this->OnComponentRemoved<Behaviour>(entity, component);
}
