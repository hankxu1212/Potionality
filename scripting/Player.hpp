#pragma once

#include <map>

#include "Behaviour.hpp"
#include "Hearts.hpp"
#include "interactables/InteractableObject.h"
#include "../scene/SceneManager.hpp"

class SpriteLoader;
class BoxCollider;

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
	float getInteractionDistance() { return m_AdditionalInteractionDistance; }
	bool getDoubleReputation() { return m_DoubleReputation; }

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
	SDL_Keycode eatKey = SDLK_e;
	void OnInteractPressed();
	void OnEatPressed();

	// State Handling ////////////////////////////////////////////////////////////////////////
	enum class State
	{
		Idle, Walk, Pickup, Smash, Eat, Deliver, InDialogue, Cut, Brew, Poisoned
	};
	State m_PlayerState;
	int m_MoveDir;
	float PlayerSpeed = 300.0f;
	float x_min = 80.0f;
	float x_max = 1660.0f;
	float y_min = 150.0f;
	float y_max = 820.0f;
	void HandleMovement();

	// Interact ////////////////////////////////////////////////////////////////////////
	float m_InteractCooldown;
	const float m_InteractCooldownMax = 2;

	// Consume //////////////////////////////////////////////////////////////////////
	float m_PotionEffectTime;
	const float m_PotionEffectTimeMax = 45;
	int m_Growth = 0;
	float m_AdditionalInteractionDistance = 0.0f; // Used for big cat
	float m_PoisonEffectTime;
	const float m_PoisonEffectTimeMax = 5;
	bool m_DoubleReputation = false; // Used for love potion

	// Message Display //////////////////////////////////////////////////////////////
	float m_MessageTimer;
	[[maybe_unused]] const float m_MessageTimerMax = 2;
	std::string currentMessage;
	void HandleMessages();

	// Inventory ////////////////////////////////////////////////////////////////////////
	// Note: Can only hold one ingredient or potion at a time
	InteractableObject* m_Held = nullptr;

	Hearts* m_Hearts;

	// Handling Functions ////////////////////////////////////////////////////////////////////////
	void HandleAnimations();
	void HandleAbilityCooldowns();

	// Physics ////////////////////////////////////////////////////////////////////////
	BoxCollider* playerCollider;
};
