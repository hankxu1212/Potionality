#pragma once

#include "interactables/InteractableObject.h"

class Ingredient;

class CuttingStation : public InteractableObject
{
public:
    void Awake() override;
    void Shutdown() override;
    void Update() override;
    void Interact() override;
    
    const char* getClassName() const override { return "CuttingStation"; }
private:
    bool isEmpty = true;
    Ingredient* storedIngredient;
};
