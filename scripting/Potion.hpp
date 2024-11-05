#pragma once

#include "Behaviour.hpp"
#include "PotionCore.hpp"
#include "interactables/InteractableObject.h"

class Potion : public InteractableObject
{
public:
	void Awake() override;
	void Shutdown() override;
	void Update() override;
	void Interact(InteractPayload* payload = nullptr) override;

	const char* getClassName() const override { return "Potion"; }

    std::string name;

};