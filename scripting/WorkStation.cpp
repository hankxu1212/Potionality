#include "WorkStation.hpp"

#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "Ingredient.hpp"
#include "PotionCore.hpp"

#include <cstring>

void WorkStation::Awake()
{
    InteractableObject::Awake();
    isEmpty = true;

    // Set station type based on entity name specified in scene json
    if (entity->name().compare("CuttingStation") == 0) {
        stationType = StationType::Cut;
    } else if (entity->name().compare("SmashingStation") == 0) {
        stationType = StationType::Smash;
    } else if (entity->name().compare("BrewingStation") == 0) {
        stationType = StationType::Brew;
    } else {
        stationType = StationType::None;
    };
}

void WorkStation::Shutdown()
{
    InteractableObject::Shutdown(); // need to call baseclass explicitly
}

void WorkStation::Update()
{
    InteractableObject::Update(); // need to call baseclass explicitly

    // If ingredient was picked up by player, workstation is empty again
    if (storedIngredient != nullptr && !storedIngredient->GetStored()) {
        isEmpty = true;
        storedIngredient = nullptr;
    }

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

void WorkStation::Interact(InteractPayload* payload)
{
    LOG_INFO("Interacted with work station!");
    if (Player::Instance) {
        InteractableObject* heldObject = Player::Instance->getHeldObject();

        // If player's hands are empty and table is full, process ingredient
        if (!isEmpty && heldObject == nullptr) {
            switch (stationType) {
                case StationType::Cut:
                    Player::Instance->cut();
                    storedIngredient->process(Action::Cut);
                    break;
                case StationType::Smash:
                    Player::Instance->smash();
                    storedIngredient->process(Action::Smash);
                    break;
                case StationType::Brew:
                    storedIngredient->process(Action::Brew);
                    break;
                default:
                    LOG_WARN("Station type invalid");
                    break;
            }
        } else if (isEmpty && heldObject != nullptr) {
            const char* objectName = heldObject->getClassName();
            if (std::strcmp(objectName, "Ingredient") != 0) {
                LOG_INFO("Can't put potions on table (for now)");
            } else {
                Ingredient* ingredient = dynamic_cast<Ingredient*>(heldObject);
                assert(ingredient && "held object is asserted to be an ingredient!");

                this->storedIngredient = ingredient;
                LOG_INFO("Place ingredient on table");
                ingredient->SetHeld(false);
                ingredient->SetStored(true);
                Player::Instance->removeHeldObject();
                storedIngredient->GetTransform()->SetPosition(GetTransform()->position().x + 60, GetTransform()->position().y + 70);
                isEmpty = false;
            }
        } else if (!isEmpty) {
            LOG_INFO("Can only have one ingredient on table");
        } else {
            LOG_INFO("No item to work with");
        }
    }
}

SETUP_DEFAULT_CALLBACKS(WorkStation)
