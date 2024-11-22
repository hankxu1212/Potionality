#include "Player.hpp"
#include "../scene/Scene.hpp"
#include "../scene/SceneManager.hpp"
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

	Entity* hearts = SceneManager::Get()->getScene()->Instantiate("Hearts", GetTransform()->position(), glm::vec2{32, 32}, 0, 1);
	hearts->AddComponent<SpriteLoader>(true, "sprtShader");
	hearts->AddComponent<SpriteRenderer>(false, "hearts");
	hearts->AddComponent<Hearts>(true);
	m_Hearts = hearts->GetComponent<Hearts>();

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
		m_PlayerState == State::Smash || m_PlayerState == State::Brew || m_PlayerState == State::Eat || m_PlayerState == State::Poisoned)
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
		m_PlayerState == State::Eat || m_PlayerState == State::Poisoned)
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
				m_PlayerState = State::Pickup;
			}
			else if (m_Held != nullptr && obj->GetEntityID() == m_Held->GetEntityID()) {
				m_Held = nullptr;
				m_PlayerState = State::Pickup;
			}
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
		m_PlayerState == State::Eat || m_PlayerState == State::Poisoned)
		return;
	
	if (m_Held != nullptr) {
		const char* objectName = m_Held->getClassName();
		if (std::strcmp(objectName, "Potion") == 0) {
			Potion* potion = dynamic_cast<Potion*>(m_Held);
			if (potion->name == "red_potion") {
				x_min = 20.0f;
				x_max = 1480.0f;
				y_min = -100.0f;
				y_max = 570.0f;
				BoxCollider* boxCollider = entity->GetComponent<BoxCollider>();
				boxCollider->setOffset(glm::vec2{142, 312});
				m_Growth = 1;
				entity->transform()->SetSize(glm::vec2{240, 320});
				glm::vec2 offset = glm::vec2{-24, -64};
				if (!ColliderManager::Get()->CheckCollisionFuture(playerCollider, offset))
					GetTransform()->Translate(offset);
				m_AdditionalInteractionDistance = 100.0f;
				m_Hearts->setPlayerBig(true);
			} else if (potion->name == "blue_potion") {
				PlayerSpeed = 1000.0f;
			} else if (potion->name == "green_potion") {
				SpritesheetRenderer* spritesheetRenderer = entity->GetComponent<SpritesheetRenderer>();
				spritesheetRenderer->SetActive(false);
			} else if (potion->name == "love_potion") {
				SpriteRenderer* heartRenderer = m_Hearts->entity->GetComponent<SpriteRenderer>();
				heartRenderer->SetActive(true);
				// TODO: Double customer payment/reputation
			} else if (potion->name == "poison_potion") {
				PlayerSpeed = 0.0f;
				m_PoisonEffectTime = m_PoisonEffectTimeMax;
			}
			potion->SetHeld(false);
			potion->SetStored(true);
			SceneManager::Get()->getScene()->Destroy(size_t(m_Held->GetEntityID()));
			m_Held = nullptr;
			m_PlayerState = State::Eat;
			m_InteractCooldown = m_InteractCooldownMax;
			m_PotionEffectTime = m_PotionEffectTimeMax;
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
	GetTransform()->SetPositionX(std::clamp(GetTransform()->position().x, x_min, x_max));
	GetTransform()->SetPositionY(std::clamp(GetTransform()->position().y, y_min, y_max));
}

void Player::HandleAbilityCooldowns()
{
	if (m_PlayerState == State::Pickup || m_PlayerState == State::Cut || m_PlayerState == State::Smash || m_PlayerState == State::Brew || 
		m_PlayerState == State::Eat) {
		if (m_InteractCooldown > 0) {
			m_InteractCooldown -= Time::DeltaTime;
			if (m_InteractCooldown <= 0)
				m_PlayerState = State::Idle;
		}
	} else if (m_PlayerState == State::Poisoned) {
		if (m_PoisonEffectTime > 0) {
			m_PoisonEffectTime -= Time::DeltaTime;
			if (m_PoisonEffectTime <= 0) {
				m_PlayerState = State::Idle;
				PlayerSpeed = 300.0f;
			}
		}
	}

	if (m_PotionEffectTime > 0) {
		m_PotionEffectTime -= Time::DeltaTime;
		if (m_Growth > 0 && m_Growth < 5) {
			m_Growth++;
			entity->transform()->SetSize(entity->transform()->size() + glm::vec2{48, 64});
			glm::vec2 offset = glm::vec2{-24, -64};
			if (!ColliderManager::Get()->CheckCollisionFuture(playerCollider, offset))
				GetTransform()->Translate(offset);
		}
		if (PlayerSpeed == 0.0f && m_PlayerState == State::Idle) { // Only set player to poisoned after "eat" animation finishes
			m_PlayerState = State::Poisoned;
		}
		if (m_PotionEffectTime <= 0) {
			PlayerSpeed = 300.0f;
			
			m_Growth = -5;
			x_min = 80.0f;
			x_max = 1660.0f;
			y_min = 150.0f;
			y_max = 820.0f;
			BoxCollider* boxCollider = entity->GetComponent<BoxCollider>();
			boxCollider->setOffset(glm::vec2{46, 56});
			m_AdditionalInteractionDistance = 0.0f;

			SpritesheetRenderer* spritesheetRenderer = entity->GetComponent<SpritesheetRenderer>();
			spritesheetRenderer->SetActive(true);

			SpriteRenderer* heartRenderer = m_Hearts->entity->GetComponent<SpriteRenderer>();
			heartRenderer->SetActive(false);
			m_Hearts->setPlayerBig(false);
		}
	} else if (m_Growth < 0) {
		m_Growth++;
		entity->transform()->SetSize(entity->transform()->size() - glm::vec2{48, 64});
		glm::vec2 offset = glm::vec2{24, 64};
		if (!ColliderManager::Get()->CheckCollisionFuture(playerCollider, offset))
			GetTransform()->Translate(offset);
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
	
	case State::Poisoned:
		// TODO: Poisoned animation
		playerSprite->SetLoopRegion(14, 4);
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
