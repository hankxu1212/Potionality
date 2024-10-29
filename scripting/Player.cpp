#include "Player.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "../renderText.hpp"
#include "Ingredient.hpp"
#include "Customer.hpp"
#include "Potion.hpp"
#include "../sound/SoundManager.hpp"

#include <array>

SpritesheetRenderer* playerSprite;

void Player::Awake()
{
	if (Player::Instance)
	{
		LOG_WARN("Multiple player instances found!");
	}

	Player::Instance = this;

	playerSprite = entity->GetComponent<SpritesheetRenderer>();

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
	if (m_PlayerState == State::Smash)
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
		if (evt.key.keysym.sym == SDLK_LEFT) {
			negX.downs += 1;
			negX.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			posX.downs += 1;
			posX.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			negY.downs += 1;
			negY.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			posY.downs += 1;
			posY.pressed = true;
			return true;
		}else if (evt.key.keysym.sym == interactKey) {
			OnInteractPressed();
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			negX.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			posX.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			negY.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_DOWN) {
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
	if (m_PlayerState == State::Smash)
		return; // if already smashing, dont do anythin g

	// TODO: Will probably need better method than looping once we have more ingredients
	for (Ingredient* ingredient : Ingredient::Instances) {
		auto& ingredientPos = ingredient->GetTransform()->position;
		float distance = glm::distance(GetTransform()->position, ingredientPos);
		if (distance < interactionDistance && ingredient->GetActive())
		{
			// Add ingredient to inventory
			SpriteRenderer* ingredientSprite = ingredient->entity->GetComponent<SpriteRenderer>();
			const std::string& ingredientName = ingredientSprite->spriteToDraw;
			if (m_Inventory.count(ingredientName)) m_Inventory[ingredientName]++;
			else m_Inventory[ingredientName] = 1;

			// Remove ingredient
			ingredientSprite->Deactivate();
			ingredient->Deactivate();

			// Play sfx
			SoundManager::Get()->PlayOneShot("BiteSFX");
		}
	}

	Customer* customer = Customer::Instance;
	auto& customerPos = customer->GetTransform()->position;
	float distance = glm::distance(GetTransform()->position, customerPos);
	if (distance < interactionDistance) {
		// If player has 1 of every ingredient, playtest potion is deliverable
		// TODO: Change hardcoded values, implement different potion recipes
		if (m_Inventory.count("flower") && m_Inventory.count("mushroom") && m_Inventory.count("purple_quartz") && m_Inventory.count("white_quartz")) {
			Potion* potion = Potion::Instance;
			SpriteRenderer* potionSprite = potion->entity->GetComponent<SpriteRenderer>();
			potionSprite->Activate();
			currentMessage = "Delivered potion! You win!";
			m_Inventory.clear(); // Note: Later on, we'll need to decrement the relevant ingredients in the player's inventory instead of just clearing

			// Play sfx
			SoundManager::Get()->PlayOneShot("SuccessSFX");
		} else {
			currentMessage = "Missing ingredients!";
		}
		m_MessageTimer = m_MessageTimerMax;
	}

	m_PlayerState = State::Smash;
	m_SmashCooldown = m_SmashCooldownMax;
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
		m_PlayerState = State::Walk;
		GetTransform()->position += PlayerSpeed * Time::DeltaTime * DIRECTIONS[m_MoveDir];
	}
	else
	{
		m_PlayerState = State::Idle;
	}
}

void Player::HandleAbilityCooldowns()
{
	if (m_PlayerState == State::Smash) 
	{
		if (m_SmashCooldown > 0) {
			m_SmashCooldown -= Time::DeltaTime;
			if (m_SmashCooldown <= 0)
				m_PlayerState = State::Idle;
		}
	}
}

void Player::HandleAnimations()
{
	if (m_PlayerState == State::Idle)
	{
		playerSprite->SetLoopRegion(0, 3);
	}
	else if (m_PlayerState == State::Walk)
	{
		playerSprite->SetLoopRegion(m_MoveDir + 2, 3);
	}
	else if (m_PlayerState == State::Smash)
	{
		playerSprite->SetLoopRegion(1, 4);
	}
}

void Player::HandleMessages()
{
	if (m_MessageTimer > 0) {
		m_MessageTimer -= Time::DeltaTime;
		RenderText(currentMessage, GetTransform()->position.x, 1080 - GetTransform()->position.y);
	}
}

void Player::DEBUG()
{
	if (m_DebugPeriodCounter <= 0) 
	{
		if (m_Inventory.size() > 0) {
			LOG_INFO("Current inventory counts:");
			for (const auto& [key, val] : m_Inventory) {
				LOG_INFO(key + ':' + std::to_string(val));
			}
		}
		m_DebugPeriodCounter = m_DebugPeriodCounterMax;
	}
	m_DebugPeriodCounter -= Time::DeltaTime;
}

template<>
void Scene::OnComponentAdded<Player>(Entity& entity, Player& component)
{
	this->OnComponentAdded<Behaviour>(entity, component);
}

template<>
void Scene::OnComponentRemoved<Player>(Entity& entity, Player& component)
{
	this->OnComponentRemoved<Behaviour>(entity, component);
}