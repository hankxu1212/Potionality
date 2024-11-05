#pragma once

#include "../Behaviour.hpp"

class SpriteRenderer;
class SpritesheetRenderer;

struct InteractPayload {
	bool isInDialogue = false;
};

class InteractableObject : public Behaviour
{
public:
	virtual void Interact(InteractPayload* payload=nullptr) {}

	void SetActive();

	void SetInactive();

	bool GetHoldable() { return isHoldable; }
	bool GetHeld() { return isHeld; }
    void SetHeld(bool heldstate) { isHeld = heldstate; }

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

