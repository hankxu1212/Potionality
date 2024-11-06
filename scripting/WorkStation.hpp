#pragma once

#include <unordered_map>

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
		Cut, Smash, Brew, None
	};

    bool isEmpty = true;
    Ingredient* storedIngredient;
    std::unordered_map<std::string, int> ingredientCounts; // Only used for brewing stations
    StationType stationType;
};
