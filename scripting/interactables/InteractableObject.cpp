#include "InteractableObject.h"

#include "InteractableManager.h"
#include "../../renderText.hpp"

void InteractableObject::Awake()
{
	InteractableManager::Get()->Add(this);
}

void InteractableObject::Shutdown()
{
	InteractableManager::Get()->Remove(this);
}

void InteractableObject::Update()
{
	if (isCurrentInteractable)
	{
		RenderText("Interact", GetTransform()->position.x, 1080 - GetTransform()->position.y);
	}
}

void InteractableObject::SetActive()
{
	isCurrentInteractable = true;
}

void InteractableObject::SetInactive()
{
	isCurrentInteractable = false;
}
