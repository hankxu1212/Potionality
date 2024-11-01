#pragma once
#include "Behaviour.hpp"
#include "PotionCore.hpp"
#include "interactables/InteractableObject.h"
#include <unordered_map>

class CuttingStation : public InteractableObject
{
public:
    void Awake() override;
    void Shutdown() override;
    void Update() override;
    void Interact() override;
    
    const char* getClassName() const override { return "CuttingStation"; }
private:
    bool isEmpty;
    InteractableObject* storedIngredient;
};
