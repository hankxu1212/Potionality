#pragma once

#include "Behaviour.hpp"

class SpriteRenderer;

class Player : public Behaviour
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void Shutdown() override;
	bool HandleEvent(const SDL_Event& e) override;

	const char* getClassName() const override { return "Player"; }

private:
    //input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	SpriteRenderer* spriteRenderer;
};