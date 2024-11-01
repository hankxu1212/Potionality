#include "CuttingStation.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "Ingredient.hpp"
#include "../PlayMode.hpp"
#include "PotionShop.hpp"
#include "interactables/InteractableManager.h"
#include <iostream>

void CuttingStation::Awake()
{
    InteractableObject::Awake();
    isEmpty = true;
}
void CuttingStation::Shutdown()
{
    InteractableObject::Shutdown(); // need to call baseclass explicitly
}
void CuttingStation::Update()
{
    InteractableObject::Update(); // need to call baseclass explicitly
    /*
     ActionState& state = ingredientStates[ingredient];
     if (!state.isComplete) {
         state.progress += deltaTime;  // Update progress
         if (state.progress >= 1.0f) {  // Assuming 1.0f is completion threshold
             state.progress = 1.0f;
             state.isComplete = true;
             completeAction(ingredient);
         }
     }
     */
   
}

void CuttingStation::Interact()
{
    LOG_INFO("Interacted with work station!");
    if (Player::Instance) {
        InteractableObject* heldObject = Player::Instance->getHeldObject();
        Ingredient* ingredient = dynamic_cast<Ingredient*>(heldObject);
        if (isEmpty && ingredient) {
            this->storedIngredient = ingredient;
            LOG_INFO("Place ingredient on table");
            ingredient->SetHeld(false);
            Player::Instance->removeHeldObject();
            storedIngredient -> GetTransform()->SetPosition(GetTransform()->position().x + 60, GetTransform()->position().y + 70);
            //ingredient->process();
        } else {
            LOG_INFO("Can only have one ingredient on table");
        }
    }
}

SETUP_DEFAULT_CALLBACKS(CuttingStation)
