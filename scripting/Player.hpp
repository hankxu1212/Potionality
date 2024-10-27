#pragma once

#include <list>

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

	int GetSmashes();
	void AddIngredient(uint64_t ingredient_id);
	std::list<uint64_t> GetIngredients();

private:
    //input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, interact;

	SDL_Keycode interactKey = SDLK_SPACE;
	void OnInteractPressed();

	enum class State
	{
		Idle, Walk, Smash, Eat, Deliver
	};

	[[maybe_unused]] State m_PlayerState;
	std::list<uint64_t> ingredients;

	void HandleMovement();

	void HandleState();
};