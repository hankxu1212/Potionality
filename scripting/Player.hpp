#pragma once

#include <map>

#include "Behaviour.hpp"

class SpriteLoader;

class Player : public Behaviour
{
public:
	// global player instance
	inline static Player* Instance;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Shutdown() override;
	bool HandleEvent(const SDL_Event& e) override;

	const char* getClassName() const override { return "Player"; }

	const int GetCount(const std::string& itemName) {
		if (m_Inventory.count(itemName)) return m_Inventory[itemName];
		else return 0;
	}

private:
	// Debug ////////////////////////////////////////////////////////////////////////
	// refreshes the debug print every 1 second
	void DEBUG();
	float m_DebugPeriodCounter;
	const float m_DebugPeriodCounterMax = 1;

	// Interaction Handling ////////////////////////////////////////////////////////////////////////
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} negX, posX, posY, negY;

	void HandleInputReset();

	SDL_Keycode interactKey = SDLK_SPACE;
	void OnInteractPressed();

	float interactionDistance = 100;

	// State Handling ////////////////////////////////////////////////////////////////////////
	enum class State
	{
		Idle, Walk, Smash, Eat, Deliver
	};
	State m_PlayerState;
	int m_MoveDir;
	const float PlayerSpeed = 300.0f;
	void HandleMovement();

	// Smash ////////////////////////////////////////////////////////////////////////
	float m_SmashCooldown;
	const float m_SmashCooldownMax = 2;

	// Message Display //////////////////////////////////////////////////////////////
	float m_MessageTimer;
	const float m_MessageTimerMax = 2;
	std::string currentMessage;
	void HandleMessages();

	// Inventory ////////////////////////////////////////////////////////////////////////
	std::map<const std::string, int> m_Inventory;

	// Handling Functions ////////////////////////////////////////////////////////////////////////
	void HandleAnimations();
	void HandleAbilityCooldowns();
};