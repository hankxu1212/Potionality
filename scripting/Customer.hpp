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

	const char* getClassName() const override { return "Customer"; }

	void Initialize(CustomerInfo info);

private:
	CustomerInfo m_CustomerInfo;

	float m_MovementTimer;
	int walkDir = -1;
	float walkSpeedPerSecond = 100;
	bool destroyAfterMove = false;

	enum class State
	{
		Idle, Walk
	};
	State m_CustomerState;

	void HandleAnimations();

	SpritesheetRenderer* customerSprite;

	std::string currentInteractionString;
};