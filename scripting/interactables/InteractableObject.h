#pragma once

#include "../Behaviour.hpp"

class SpriteRenderer;
class SpritesheetRenderer;

class InteractableObject : public Behaviour
{
public:
	virtual void Interact() {}

	void SetActive();

	void SetInactive();

	bool GetHoldable() { return isHoldable; }
	bool GetHeld() { return isHeld; }

protected:
	void Awake() override;
	void Shutdown() override;
	void Update() override;

	bool isCurrentInteractable = false;

	SpriteRenderer* spriteRenderer;
	[[maybe_unused]] SpriteRenderer* spritesheetRenderer;
	bool isHoldable = false;
	bool isHeld = false;
};

