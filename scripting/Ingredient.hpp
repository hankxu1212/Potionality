#pragma once

#include "Behaviour.hpp"
#include "PotionCore.hpp"
#include "interactables/InteractableObject.h"

class Ingredient : public InteractableObject
{
public:
	void Awake() override;
	void Shutdown() override;
	void Update() override;
	void Interact() override;

	const char* getClassName() const override { return "Ingredient"; }

    Ingredient_T* ingredient;

};