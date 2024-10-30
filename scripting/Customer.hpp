#pragma once

#include "Behaviour.hpp"
#include "PotionCore.hpp"
#include "interactables/InteractableObject.h"

class Customer : public InteractableObject
{
public:
	void Awake() override;
	void Shutdown() override;

	void Interact() override;

	const char* getClassName() const override { return "Customer"; }

private:
	std::vector<PotionRequest> requests;
	float patience;
};