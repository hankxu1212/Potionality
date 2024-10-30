#include "Ingredient.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "../PlayMode.hpp"
#include "../renderText.hpp"

void Ingredient::Awake()
{
	Instances.emplace_back(this);
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
		RenderText("Chomp", GetTransform()->position.x, 1080 - GetTransform()->position.y);
	}
}

void Ingredient::Shutdown()
{
	auto it = std::find(Instances.begin(), Instances.end(), this);
	if (it != Ingredient::Instances.end()) {
		Ingredient::Instances.erase(it);  // Removes the first occurrence of `ingredient`
	}

	LOG_INFO("shut down an ingredient component");
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
