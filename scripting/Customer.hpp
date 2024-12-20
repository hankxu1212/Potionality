#pragma once

#include "Behaviour.hpp"
#include "PotionCore.hpp"
#include "interactables/InteractableObject.h"
#include "CustomerInfo.hpp"

class SpritesheetRenderer;

class Customer : public InteractableObject
{
public:
	void Awake() override;
	void Shutdown() override;
	void Update() override;
	void Interact(InteractPayload* payload = nullptr) override;

	void HandleCorrectInteract(bool doubleReputation);
	void HandleIncorrectInteract(bool doubleReputation);

	const char* getClassName() const override { return "Customer"; }

	void Initialize(CustomerInfo& info);

	uint32_t slot;

private:
	CustomerInfo m_CustomerInfo;

	float m_MovementTimer;
	int walkDir = -1;
	float walkSpeedPerSecond = 100;
	bool destroyAfterMove = false;
	int patience;

	enum class State
	{
		Idle, WalkForward, WalkBackward
	};
	State m_CustomerState;

	void HandleAnimations();

	SpritesheetRenderer* customerSprite;

	std::string currentInteractionString;
};