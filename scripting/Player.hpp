#pragma once

#include <map>

#include "Behaviour.hpp"
#include "interactables/InteractableObject.h"

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
    InteractableObject* getHeldObject() const { return m_Held; }
    void removeHeldObject() { m_Held = nullptr; }
	void cut() { m_PlayerState = State::Cut; }
	void smash() { m_PlayerState = State::Smash; }
	void brew() { m_PlayerState = State::Brew; }

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

	// State Handling ////////////////////////////////////////////////////////////////////////
	enum class State
	{
		Idle, Walk, Pickup, Smash, Eat, Deliver, InDialogue, Cut, Brew
	};
	State m_PlayerState;
	int m_MoveDir;
	const float PlayerSpeed = 300.0f;
	void HandleMovement();

	// Interact ////////////////////////////////////////////////////////////////////////
	float m_InteractCooldown;
	const float m_InteractCooldownMax = 2;

	// Message Display //////////////////////////////////////////////////////////////
	float m_MessageTimer;
	[[maybe_unused]] const float m_MessageTimerMax = 2;
	std::string currentMessage;
	void HandleMessages();

	// Inventory ////////////////////////////////////////////////////////////////////////
	// Note: Can only hold one ingredient or potion at a time
	InteractableObject* m_Held = nullptr;


	// Handling Functions ////////////////////////////////////////////////////////////////////////
	void HandleAnimations();
	void HandleAbilityCooldowns();
};
