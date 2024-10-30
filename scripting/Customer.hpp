#pragma once

#include "Behaviour.hpp"
#include "PotionCore.hpp"
#include "interactables/InteractableObject.h"

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
	std::vector<PotionRequest> requests;
	float patience;

	// display initial request
	float m_InitialRequestTimer;
	const float m_InitialRequestTimerMax = 4;
};