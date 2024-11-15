#include "Player.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "../renderText.hpp"
#include "Customer.hpp"
#include "../sound/SoundManager.hpp"
#include "interactables/InteractableManager.h"
#include "../physics/ColliderManager.hpp"

#include <array>
#include <string>
#include <algorithm>

SpritesheetRenderer* playerSprite;

void Player::Awake()
{
	LOG_INFO("Instantiated player!");
	if (Player::Instance)
	{
		LOG_WARN("Multiple player instances found!");
	}

	Player::Instance = this;

	playerSprite = entity->GetComponent<SpritesheetRenderer>();
	playerCollider = entity->GetComponent<BoxCollider>();
	assert(playerCollider);

	SoundManager::Get()->SetFollowListener(GetTransform());
}

void Player::Start()
{
}

void Player::Update()
{
	DEBUG();

	HandleAnimations();

	HandleMessages();
	
	HandleAbilityCooldowns();
	if (m_PlayerState == State::InDialogue || m_PlayerState == State::Pickup || m_PlayerState == State::Cut || 
		m_PlayerState == State::Smash || m_PlayerState == State::Brew || m_PlayerState == State::Eat)
		return;

	HandleMovement();

	HandleInputReset();
}

void Player::Shutdown()
{
}

bool Player::HandleEvent(const SDL_Event &evt)
{
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT || evt.key.keysym.sym == SDLK_a) {
			negX.downs += 1;
			negX.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT || evt.key.keysym.sym == SDLK_d) {
			posX.downs += 1;
			posX.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP || evt.key.keysym.sym == SDLK_w) {
			negY.downs += 1;
			negY.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN || evt.key.keysym.sym == SDLK_s) {
			posY.downs += 1;
			posY.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == interactKey) {
			OnInteractPressed();
			return true;
		} else if (evt.key.keysym.sym == eatKey) {
			OnEatPressed();
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT || evt.key.keysym.sym == SDLK_a) {
			negX.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT || evt.key.keysym.sym == SDLK_d) {
			posX.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP || evt.key.keysym.sym == SDLK_w) {
			negY.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_DOWN || evt.key.keysym.sym == SDLK_s) {
			posY.pressed = false;
			return true;
		}
	}

	return false;
}

void Player::HandleInputReset()
{
	// reset buttons
	negX.downs = 0;
	posX.downs = 0;
	negY.downs = 0;
	posY.downs = 0;
}

void Player::OnInteractPressed()
{
	// if in dialogue, continue conversation
	if (m_PlayerState == State::InDialogue) {
		// if cannot continue, end conversation
		if (!DialogueUI::Instance->CanContinue()) 
		{
			DialogueUI::Instance->Disable();
			m_PlayerState = State::Idle;
			return;
		}

		// other wise, continue conversation
		DialogueUI::Instance->ContinueConversation();
		return;
	}

	// if already mid-interaction, dont do anything
	if (m_PlayerState == State::Pickup || m_PlayerState == State::Cut || m_PlayerState == State::Smash || m_PlayerState == State::Brew ||
		m_PlayerState == State::Eat)
		return;

	InteractableObject* obj = InteractableManager::Get()->GetClosestObject();
	if (!obj) {
		LOG_WARN("DID not find anything to interact with!");
		return;
	}

	// you may overload the interaction function!
	InteractPayload payload;
	obj->Interact(&payload);
	if (payload.isDestroyed)
	{
		m_Held = nullptr;
		return;
	}

	// we interacted with a customer!
	if (payload.isInDialogue)
	{
		m_PlayerState = State::InDialogue;
	}
	// we interacted with a holdable/actionable item!
	else 
	{
		if (obj->GetHoldable()) { // Potions and ingredients should be holdable
			if (obj->GetHeld()) {
                std::cout << m_Held;
				m_Held = obj;
			}
			else {
				m_Held = nullptr;
			}
			m_PlayerState = State::Pickup;
		}

		m_InteractCooldown = m_InteractCooldownMax;
	}
}

void Player::OnEatPressed() {
	// if in dialogue, continue conversation
	if (m_PlayerState == State::InDialogue) {
		// if cannot continue, end conversation
		if (!DialogueUI::Instance->CanContinue()) 
		{
			DialogueUI::Instance->Disable();
			m_PlayerState = State::Idle;
			return;
		}

		// other wise, continue conversation
		DialogueUI::Instance->ContinueConversation();
		return;
	}

	// if already mid-interaction, dont do anything
	if (m_PlayerState == State::Pickup || m_PlayerState == State::Cut || m_PlayerState == State::Smash || m_PlayerState == State::Brew || 
		m_PlayerState == State::Eat)
		return;
	
	if (m_Held != nullptr) {
		const char* objectName = m_Held->getClassName();
		if (std::strcmp(objectName, "Potion") == 0) {
			SceneManager::Get()->getScene()->Destroy(size_t(m_Held->GetEntityID()));
			m_Held = nullptr;
			m_PlayerState = State::Eat;
			PlayerSpeed = 1000.0f;
			m_InteractCooldown = m_InteractCooldownMax;
			m_PotionEffectTime = m_PotionEffectTimeMax;
			// TODO: Change effects based on specific potion
		} else {
			LOG_INFO("Held item is not consumable!");
		}
	} else {
		LOG_INFO("No item to consume!");
	}
}

static std::array<glm::vec2, 8> DIRECTIONS = 
{
	normalize(glm::vec2(1, 0)),     // Right		- 0
	normalize(glm::vec2(1, 1)),    // Down-Right	- 1
	normalize(glm::vec2(0, 1)),    // Down			- 2
	normalize(glm::vec2(-1, 1)),   // Down-Left	- 3 
	normalize(glm::vec2(-1, 0)),    // Left			- 4
	normalize(glm::vec2(-1, -1)),    // Up-Left		- 5
	normalize(glm::vec2(0, -1)),     // Up			- 6
	normalize(glm::vec2(1, -1))      // Up-Right		- 7
};

void Player::HandleMovement()
{
	// index into DIRECTIONS
	m_MoveDir = -1;

	if (negX.pressed) m_MoveDir = 4;       // Left
	if (posX.pressed) m_MoveDir = 0;      // Right

	if (negY.pressed) {
		switch (m_MoveDir) {
		case 0: m_MoveDir = 7; break;  // Right + Down
		case 4: m_MoveDir = 5; break;  // Left + Down
		default: m_MoveDir = 6; break; // Down
		}
	}
	else if (posY.pressed) {
		switch (m_MoveDir) {
		case 0: m_MoveDir = 1; break;  // Right + Up
		case 4: m_MoveDir = 3; break;  // Left + Up
		default: m_MoveDir = 2; break; // Up
		}
	}

	// move dir
	if (m_MoveDir != -1)
	{
		glm::vec2 offset = PlayerSpeed * Time::DeltaTime * DIRECTIONS[m_MoveDir];
		if (!ColliderManager::Get()->CheckCollisionFuture(playerCollider, offset))
		{
			m_PlayerState = State::Walk;
			GetTransform()->Translate(offset);
		}
	}
	else
	{
		m_PlayerState = State::Idle;
	}

	// clamp location
	GetTransform()->SetPositionX(std::clamp(GetTransform()->position().x, 80.0f, 1660.0f));
	GetTransform()->SetPositionY(std::clamp(GetTransform()->position().y, 150.0f, 820.0f));
}

void Player::HandleAbilityCooldowns()
{
	if (m_PlayerState == State::Pickup || m_PlayerState == State::Cut || m_PlayerState == State::Smash || m_PlayerState == State::Brew || 
		m_PlayerState == State::Eat) 
	{
		if (m_InteractCooldown > 0) {
			m_InteractCooldown -= Time::DeltaTime;
			if (m_InteractCooldown <= 0)
				m_PlayerState = State::Idle;
		}
	}

	if (m_PotionEffectTime > 0) {
		m_PotionEffectTime -= Time::DeltaTime;
		if (m_PotionEffectTime <= 0) PlayerSpeed = 300.0f;
	}
}

void Player::HandleAnimations()
{
	switch (m_PlayerState)
	{
	case State::Idle:
		playerSprite->SetLoopRegion(0, 3);
		break;

	case State::Walk:
		playerSprite->SetLoopRegion(m_MoveDir + 2, 3);
		break;

	case State::Pickup:
		playerSprite->SetLoopRegion(1, 4);
		break;
	
	case State::Eat:
		playerSprite->SetLoopRegion(1, 4);
		break;

	case State::Cut:
		playerSprite->SetLoopRegion(12, 6);
		break;

	case State::Smash:
		playerSprite->SetLoopRegion(11, 6);
		break;

	case State::Brew:
		playerSprite->SetLoopRegion(13, 4);
		break;

	default:
		// Handle unexpected states if necessary
		break;
	}

}

void Player::HandleMessages()
{
	if (m_MessageTimer > 0) {
		m_MessageTimer -= Time::DeltaTime;
		TextRenderer::Get()->RenderText(currentMessage, GetTransform()->position().x, 1080 - GetTransform()->position().y);
	}
}

void Player::DEBUG()
{
	if (m_DebugPeriodCounter <= 0) 
	{
		if (m_Held == nullptr) LOG_INFO("No item held!");
		// else LOG_INFO(m_Held->getClassName());
		/*
		LOG_INFO("Current position:");
		LOG_INFO(GetTransform()->position().x);
		LOG_INFO(GetTransform()->position().y);
		*/
		m_DebugPeriodCounter = m_DebugPeriodCounterMax;
	}
	m_DebugPeriodCounter -= Time::DeltaTime;
}

SETUP_DEFAULT_CALLBACKS(Player)
