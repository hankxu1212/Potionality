#pragma once

#include <vector>

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

	std::vector<UUID> GetInventory();

private:
    //input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, interact;

	SDL_Keycode interactKey = SDLK_SPACE;
	void OnInteractPressed();

	float interactionDistance = 100;

	enum class State
	{
		Idle, Walk, Smash, Eat, Deliver
	};

	[[maybe_unused]] State m_PlayerState;
	std::vector<UUID> inventory;

	void HandleMovement();

	void HandleState();
};