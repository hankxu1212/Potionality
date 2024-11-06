#pragma once

#include "interactables/InteractableObject.h"

class Ingredient;

class WorkStation : public InteractableObject
{
public:
    void Awake() override;
    void Shutdown() override;
    void Update() override;
    void Interact(InteractPayload* payload = nullptr) override;
    
    const char* getClassName() const override { return "WorkStation"; }
private:
    enum class StationType
	{
		Cut, Smash, Brew
	};

    bool isEmpty = true;
    Ingredient* storedIngredient;
    StationType stationType;
};
