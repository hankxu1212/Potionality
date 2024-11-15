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
	float storedDistance = FLT_MAX;
	InteractableObject* closestObj = nullptr;
	InteractableObject* storedObj = nullptr;
	InteractableObject* heldObj = nullptr;
	for (auto& [id, obj] : m_Interactables)
	{
		glm::vec2 playerCenter = Player::Instance->GetTransform()->position() + (Player::Instance->GetTransform()->size() / 2.0f);
		glm::vec2 objCenter = obj->GetTransform()->position() + (obj->GetTransform()->size() / 2.0f);
		float d = glm::distance(playerCenter, objCenter);
		if (d < closestDistance && d < interactionDistance && !obj->GetHeld() && !obj->GetStored())
		{
			closestDistance = d;
			closestObj = obj;
		}
		if (obj->GetStored())
		{
			storedObj = obj;
			storedDistance = d;
		}
		if (obj->GetHeld()) heldObj = obj;
	}
	if (closestObj != nullptr) closestInteractableObject = closestObj;
	else if (storedObj != nullptr && storedDistance < interactionDistance + 100) closestInteractableObject = storedObj;
	else closestInteractableObject = heldObj;

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
	if(!InteractableObject)
	{
		LOG_WARN("Interactable object is null!");
	}

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
