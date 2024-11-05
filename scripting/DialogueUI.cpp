#include "DialogueUI.h"

#include "../scene/Entity.hpp"
#include "../math/Math.hpp"
#include "../scene/SceneManager.hpp"
#include "../PlayMode.hpp"
#include "../sound/SoundManager.hpp"


void DialogueUI::Awake()
{
	Instance = this;
	spriteRenderer = entity->GetComponent<SpriteRenderer>();
}

void DialogueUI::Start()
{

}

void DialogueUI::Update()
{
	// disable depth test to make sure it goes on top
	glDisable(GL_DEPTH_TEST);

	if (isInAnimation)
	{
		timeSinceUpdate += Time::DeltaTime;
		if (timeSinceUpdate > animationPeriod)
		{
			timeSinceUpdate = 0;
			charactersShown++;
			SoundManager::Get()->PlayOneShot("TypeSFX", 1);
			if (charactersShown > currentText.size()) {
				isInAnimation = false;
				TextRenderer::Get()->RenderText(currentText, 100, 300, 0.002f);
				goto end;
			}
		}

		TextRenderer::Get()->RenderText(currentText, 100, 300, 0.002f, 1800, (int)charactersShown);
	}
	else
	{
		TextRenderer::Get()->RenderText(currentText, 100, 300, 0.002f, 1800);
	}

end:
	// reenable depth test
	glEnable(GL_DEPTH_TEST);
}

void DialogueUI::Shutdown()
{
}

void DialogueUI::StartConversation(const std::string& newText)
{
	currentText.assign(newText);
	charactersShown = 0;
	isInAnimation = true;
	timeSinceUpdate = 0;
	canContinue = true;
}

void DialogueUI::ContinueConversation()
{
	charactersShown = (uint32_t)currentText.size();
	isInAnimation = false;
	canContinue = false;
	// TODO: add next line?
}

bool DialogueUI::CanContinue()
{
	return canContinue;
}

void DialogueUI::Enable()
{
	isActive = true;
	spriteRenderer->isActive = true;
}

void DialogueUI::Disable()
{
	isActive = false;
	spriteRenderer->isActive = false;
}

SETUP_DEFAULT_CALLBACKS(DialogueUI)