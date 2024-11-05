#include "InteractableObject.h"

#include "InteractableManager.h"
#include "../../renderText.hpp"

#include "../../scene/Entity.hpp"
#include "../../PlayMode.hpp"
#include "../../sound/SoundManager.hpp"

void InteractableObject::Awake()
{
	InteractableManager::Get()->Add(this);
	Entity* uiEntity = entity->AddChild("Interaction UI Icon", glm::vec2{0,0}, glm::vec2{32, 32}, 0, 3);
	
	// add some sprite stuff
	uiEntity->AddComponent<SpriteLoader>(true, SPRITE_SHADER);
	spriteRenderer = &uiEntity->AddComponent<SpriteRenderer>(true, "UI_questionmark");
}

void InteractableObject::Shutdown()
{
	InteractableManager::Get()->Remove(this);
}

void InteractableObject::Update()
{
	if (isCurrentInteractable)
	{
		TextRenderer::Get()->RenderText("Interact", GetTransform()->position().x, 1080 - GetTransform()->position().y);
	}
	spriteRenderer->isActive = isCurrentInteractable;
}

void InteractableObject::SetActive()
{
	isCurrentInteractable = true;
	SoundManager::Get()->PlayOneShot("InteractAppearSFX", 0.1f);
}

void InteractableObject::SetInactive()
{
	isCurrentInteractable = false;
	//SoundManager::Get()->PlayOneShot("InteractDisappearSFX", 0.1f);
}
