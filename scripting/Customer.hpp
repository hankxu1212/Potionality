#pragma once

#include "Behaviour.hpp"
#include "PotionCore.hpp"
#include "interactables/InteractableObject.h"
#include "CustomerInfo.hpp"

class Customer : public InteractableObject
{
public:
	void Awake() override;
	void Shutdown() override;
	void Update() override;
	void Interact() override;

	const char* getClassName() const override { return "Customer"; }

	void InstantiateRequests(uint32_t numRequests);

private:
	CustomerInfo info;
	// display initial request
	float m_InitialRequestTimer;
	const float m_InitialRequestTimerMax = 4;

	enum class State
	{
		Idle, Walk
	};
	State m_CustomerState;

	void HandleAnimations();
};