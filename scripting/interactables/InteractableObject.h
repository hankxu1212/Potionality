#pragma once

#include "../Behaviour.hpp"

class SpriteRenderer;
class SpritesheetRenderer;

struct InteractPayload {
	bool isInDialogue = false;
	bool isDestroyed = false;
};

class InteractableObject : public Behaviour
{
public:
	virtual void Interact(InteractPayload* payload=nullptr) {}

	void SetActive();

	void SetInactive();

	bool GetHoldable() { return isHoldable; }
	bool GetHeld() { return isHeld; }
    void SetHeld(bool heldState) { isHeld = heldState; }
	bool GetStored() { return isStored; }
    void SetStored(bool storedState) { isStored = storedState; }

protected:
	void Awake() override;
	void Shutdown() override;
	void Update() override;

	bool isCurrentInteractable = false;

	SpriteRenderer* spriteRenderer;
	[[maybe_unused]] SpriteRenderer* spritesheetRenderer;
	bool isHoldable = false;
	bool isHeld = false;
	bool isStored = false;
};

