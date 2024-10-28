#include "Player.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Ingredient.hpp"
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
}

void Player::Start()
{
}

void Player::Update()
{
	DEBUG();

	HandleAnimations();

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
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}else if (evt.key.keysym.sym == interactKey) {
			OnInteractPressed();
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}
	}

	return false;
}

void Player::HandleInputReset()
{
	// reset buttons
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void Player::OnInteractPressed()
{
	if (m_PlayerState == State::Smash)
		return; // if already smashing, dont do anythin g

	// TODO: Will probably need better method than looping once we have more ingredients
	for (Ingredient* ingredient : Ingredient::Instances) {
		auto& ingredientPos = ingredient->GetTransform()->position;
		float distance = glm::distance(GetTransform()->position, ingredientPos);
		if (distance < interactionDistance)
		{
			m_Inventory.emplace_back(uint64_t(ingredient->GetEntityID()));
			//TODO: Remove ingredient and sprite from scene
		}
	}

	m_PlayerState = State::Smash;
	m_SmashCooldown = m_SmashCooldownMax;
}

static std::array<glm::vec2, 8> DIRECTIONS = {
	normalize(glm::vec2(1, 0)),     // Right		- 0
	normalize(glm::vec2(1, -1)),    // Down-Right	- 1
	normalize(glm::vec2(0, -1)),    // Down			- 2
	normalize(glm::vec2(-1, -1)),   // Down-Left	- 3 
	normalize(glm::vec2(-1, 0)),    // Left			- 4
	normalize(glm::vec2(-1, 1)),    // Up-Left		- 5
	normalize(glm::vec2(0, 1)),     // Up			- 6
	normalize(glm::vec2(1, 1))      // Up-Right		- 7
};


void Player::HandleMovement()
{
	// index into DIRECTIONS
	m_MoveDir = -1;

	if (left.pressed) m_MoveDir = 4;       // Left
	if (right.pressed) m_MoveDir = 0;      // Right

	if (down.pressed) {
		switch (m_MoveDir) {
		case 0: m_MoveDir = 7; break;  // Right + Down
		case 4: m_MoveDir = 5; break;  // Left + Down
		default: m_MoveDir = 6; break; // Down
		}
	}
	else if (up.pressed) {
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
		int blendedDir;

		switch (m_MoveDir) {
		case 0: // Right
		case 1: // Up-Right
		case 7: // Down-Right
			blendedDir = 5;  // Right
			break;

		case 4: // Left
		case 3: // Up-Left
		case 5: // Down-Left
			blendedDir = 4;  // Left
			break;

		case 2: // Up
			blendedDir = 2;  // Up
			break;

		case 6: // Down
			blendedDir = 3;  // Down
			break;

		default:
			blendedDir = -1; // Handle unexpected values, if needed
			break;
		}

		playerSprite->SetLoopRegion(blendedDir, 3);
	}
	else if (m_PlayerState == State::Smash)
	{
		playerSprite->SetLoopRegion(1, 4);
	}
}

void Player::DEBUG()
{
	if (m_DebugPeriodCounter <= 0) 
	{
		if (m_Inventory.size() > 0) {
			LOG_INFO("Current inventory IDs:");
			for (int i = 0; i < m_Inventory.size(); i++) {
				LOG_INFO(uint64_t(m_Inventory[i]));
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