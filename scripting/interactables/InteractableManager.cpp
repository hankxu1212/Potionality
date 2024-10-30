#include "InteractableManager.h"

#include "../../scene/Entity.hpp"
#include "../Player.hpp"

InteractableManager::InteractableManager()
{
}

InteractableManager::~InteractableManager()
{
}

void InteractableManager::Update()
{
	prevInteractableObject = closestInteractableObject;

	float closestDistance = FLT_MAX;
	InteractableObject* closestObj = nullptr;
	for (auto& [id, obj] : m_Interactables)
	{
		float d = glm::distance(Player::Instance->GetTransform()->position(), obj->GetTransform()->position());
		if (d < closestDistance && d < interactionDistance)
		{
			closestDistance = d;
			closestObj = obj;
		}
	}
	closestInteractableObject = closestObj;

	if (closestInteractableObject != prevInteractableObject)
	{
		if (prevInteractableObject)
			prevInteractableObject->SetInactive();
		if (closestInteractableObject)
			closestInteractableObject->SetActive();
	}
}

void InteractableManager::Add(InteractableObject* InteractableObject)
{
	auto it = m_Interactables.find(InteractableObject->entity->uuid());
	if (it != m_Interactables.end()) {
		LOG_WARN("Added a InteractableObject multiple times. This should never happen.");
		return;
	}

	m_Interactables[InteractableObject->entity->uuid()] = InteractableObject;
}

void InteractableManager::Remove(InteractableObject* InteractableObject)
{
	auto it = m_Interactables.find(InteractableObject->entity->uuid());
	if (it == m_Interactables.end()) {
		LOG_WARN("Tried to remove a InteractableObject that was already removed or never added. This should never happen");
		return;
	}

	m_Interactables.erase(InteractableObject->entity->uuid());
}

InteractableObject* InteractableManager::GetClosestObject() const
{
	return closestInteractableObject;
}