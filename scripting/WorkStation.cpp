#include "WorkStation.hpp"

#include "../scene/Entity.hpp"
#include "../scene/SceneManager.hpp"
#include "Player.hpp"
#include "Ingredient.hpp"
#include "PotionCore.hpp"
#include "../PlayMode.hpp"

#include <cstring>

SpritesheetRenderer* cauldronSprite;

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

        // If player's hands are empty and station has ingredients, process ingredient
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
                    Player::Instance->brew();
                    // TODO: Refactor with recipe manager
                    // Current version is hard coded for testing purposes
                    if (ingredientCounts.count("PurpleQuartzSmashedIngredient") && ingredientCounts.count("WhiteQuartzSmashedIngredient") && ingredientCounts.count("RoseCutIngredient")) {
                        ingredientCounts.clear();
                        entity->RemoveComponent<SpritesheetRenderer>();
                        entity->RemoveComponent<SpritesheetLoader>();
                        entity->AddComponent<SpriteLoader>(true, "sprtShader");
                        entity->AddComponent<SpriteRenderer>(true, "cauldron");
                        isEmpty = true;
                        glm::vec2 newPos(GetTransform()->position().x, GetTransform()->position().y - 64);
                        Entity* newPotion = SceneManager::Get()->getScene()->Instantiate("LovePotion", newPos, glm::vec2{64, 64}, 0, 1);
                        newPotion->AddComponent<SpriteLoader>(true, "sprtShader");
                        newPotion->AddComponent<SpriteRenderer>(true, "love_potion");
                        newPotion->AddComponent<Potion>(true, "love_potion");
                        entity->RemoveComponent<SpritesheetRenderer>();
                        entity->RemoveComponent<SpritesheetLoader>();
                        entity->AddComponent<SpriteLoader>(true, "sprtShader");
                        entity->AddComponent<SpriteRenderer>(true, "cauldron");
                    } else if (ingredientCounts.count("MushroomCutIngredient") && ingredientCounts.count("FlowerIngredient")) {
                        ingredientCounts.clear();
                        isEmpty = true;
                        glm::vec2 newPos(GetTransform()->position().x, GetTransform()->position().y - 64);
                        Entity* newPotion = SceneManager::Get()->getScene()->Instantiate("RedPotion", newPos, glm::vec2{64, 64}, 0, 1);
                        newPotion->AddComponent<SpriteLoader>(true, "sprtShader");
                        newPotion->AddComponent<SpriteRenderer>(true, "red_potion");
                        newPotion->AddComponent<Potion>(true, "red_potion");
                        entity->RemoveComponent<SpritesheetRenderer>();
                        entity->RemoveComponent<SpritesheetLoader>();
                        entity->AddComponent<SpriteLoader>(true, "sprtShader");
                        entity->AddComponent<SpriteRenderer>(true, "cauldron");
                    } else if (ingredientCounts.count("MushroomCutIngredient") && ingredientCounts.count("LeafIngredient")) {
                        ingredientCounts.clear();
                        isEmpty = true;
                        glm::vec2 newPos(GetTransform()->position().x, GetTransform()->position().y - 64);
                        Entity* newPotion = SceneManager::Get()->getScene()->Instantiate("GreenPotion", newPos, glm::vec2{64, 64}, 0, 1);
                        newPotion->AddComponent<SpriteLoader>(true, "sprtShader");
                        newPotion->AddComponent<SpriteRenderer>(true, "green_potion");
                        newPotion->AddComponent<Potion>(true, "green_potion");
                        entity->RemoveComponent<SpritesheetRenderer>();
                        entity->RemoveComponent<SpritesheetLoader>();
                        entity->AddComponent<SpriteLoader>(true, "sprtShader");
                        entity->AddComponent<SpriteRenderer>(true, "cauldron");
                    } else if (ingredientCounts.count("MushroomIngredient") && ingredientCounts.count("LeafIngredient") && ingredientCounts.count("PurpleQuartzSmashedIngredient")) {
                        ingredientCounts.clear();
                        isEmpty = true;
                        glm::vec2 newPos(GetTransform()->position().x, GetTransform()->position().y - 64);
                        Entity* newPotion = SceneManager::Get()->getScene()->Instantiate("RedPotion", newPos, glm::vec2{64, 64}, 0, 1);
                        newPotion->AddComponent<SpriteLoader>(true, "sprtShader");
                        newPotion->AddComponent<SpriteRenderer>(true, "poison_potion");
                        newPotion->AddComponent<Potion>(true, "poison_potion");
                        entity->RemoveComponent<SpritesheetRenderer>();
                        entity->RemoveComponent<SpritesheetLoader>();
                        entity->AddComponent<SpriteLoader>(true, "sprtShader");
                        entity->AddComponent<SpriteRenderer>(true, "cauldron");
                    }else if (ingredientCounts.count("FlowerIngredient")) {
                        ingredientCounts.clear();
                        isEmpty = true;
                        glm::vec2 newPos(GetTransform()->position().x, GetTransform()->position().y - 64);
                        Entity* newPotion = SceneManager::Get()->getScene()->Instantiate("BluePotion", newPos, glm::vec2{64, 64}, 0, 1);
                        newPotion->AddComponent<SpriteLoader>(true, "sprtShader");
                        newPotion->AddComponent<SpriteRenderer>(true, "blue_potion");
                        newPotion->AddComponent<Potion>(true, "blue_potion");
                        entity->RemoveComponent<SpritesheetRenderer>();
                        entity->RemoveComponent<SpritesheetLoader>();
                        entity->AddComponent<SpriteLoader>(true, "sprtShader");
                        entity->AddComponent<SpriteRenderer>(true, "cauldron");
                    } else {
                        LOG_INFO("Failed to brew potion!");
                    }
                    break;
                default:
                    LOG_WARN("Station type invalid.");
                    break;
            }
        } else if (stationType == StationType::Brew && heldObject != nullptr) { // Brewing station can hold more than one ingredient
            const char* objectName = heldObject->getClassName();
            if (std::strcmp(objectName, "Ingredient") != 0) {
                LOG_INFO("Can't put potions in pot (for now).");
            } else {
                Ingredient* ingredient = dynamic_cast<Ingredient*>(heldObject);
                assert(ingredient && "held object is asserted to be an ingredient!");

                LOG_INFO("Placed ingredient in pot. Note: Currently impossible to remove ingredient from pot once placed (it's dissolved).");
                ingredient->SetHeld(false);
                ingredient->SetStored(true);
                Player::Instance->removeHeldObject();
                
                if(isEmpty){
                    entity->RemoveComponent<SpriteRenderer>();
                    entity->RemoveComponent<SpriteLoader>();
                }else{
                    entity->RemoveComponent<SpritesheetRenderer>();
                    entity->RemoveComponent<SpritesheetLoader>();
                }
                entity->AddComponent<SpritesheetLoader>(true, SPRITESHEET_SHADER, 4,1);
                
                isEmpty = false;
                
                LOG_INFO(ingredient->ingredient->name);
                
                if (ingredient->ingredient->name == "MushroomCutIngredient" || ingredient->ingredient->name == "MushroomIngredient") {
                    entity->AddComponent<SpritesheetRenderer>(true, "cauldron_spritesheet_red");
                    cauldronSprite = entity->GetComponent<SpritesheetRenderer>();
                }
                else if (ingredient->ingredient->name == "LeafIngredient") {
                    entity->AddComponent<SpritesheetRenderer>(true, "cauldron_spritesheet_green");
                    cauldronSprite = entity->GetComponent<SpritesheetRenderer>();
                }
                else if (ingredient->ingredient->name == "PurpleQuartzSmashedIngredient" || ingredient->ingredient->name == "PurpleQuartzIngredient") {
                    entity->AddComponent<SpritesheetRenderer>(true, "cauldron_spritesheet_purple");
                    cauldronSprite = entity->GetComponent<SpritesheetRenderer>();
                }
                else if (ingredient->ingredient->name == "WhiteQuartzSmashedIngredient" || ingredient->ingredient->name == "WhiteQuartzIngredient") {
                    entity->AddComponent<SpritesheetRenderer>(true, "cauldron_spritesheet_blue");
                    cauldronSprite = entity->GetComponent<SpritesheetRenderer>();
                }
                else if (ingredient->ingredient->name == "FlowerIngredient") {
                    entity->AddComponent<SpritesheetRenderer>(true, "cauldron_spritesheet_white");
                    cauldronSprite = entity->GetComponent<SpritesheetRenderer>();
                }
                else if (ingredient->ingredient->name == "RoseCutIngredient" || ingredient->ingredient->name == "RoseIngredient") {
                    entity->AddComponent<SpritesheetRenderer>(true, "cauldron_spritesheet_pink");
                    cauldronSprite = entity->GetComponent<SpritesheetRenderer>();
                }
                
                cauldronSprite->SetLoopRegion(0, 4);
                    
                // Increment ingredient counts
                if (ingredientCounts.count(ingredient->ingredient->name) == 0) {
                    ingredientCounts[ingredient->ingredient->name] = 1;
                } else {
                    ingredientCounts[ingredient->ingredient->name]++;
                }

                 
                // Ingredient is dissolved, destroy it
                SceneManager::Get()->getScene()->Destroy(size_t(ingredient->GetEntityID()));
                payload->isDestroyed = true;
                
            }
        } else if (isEmpty && heldObject != nullptr) { // Place ingredient on table for non-brewing stations
            const char* objectName = heldObject->getClassName();
            if (std::strcmp(objectName, "Ingredient") != 0) {
                LOG_INFO("Can't put potions on table (for now).");
            } else {
                Ingredient* ingredient = dynamic_cast<Ingredient*>(heldObject);
                assert(ingredient && "held object is asserted to be an ingredient!");

                this->storedIngredient = ingredient;
                LOG_INFO("Placed ingredient on table.");
                ingredient->SetHeld(false);
                ingredient->SetStored(true);
                Player::Instance->removeHeldObject();
                storedIngredient->GetTransform()->SetPosition(GetTransform()->position().x + 95, GetTransform()->position().y + 35);
                isEmpty = false;
            }
        } else if (!isEmpty) { // Non-brewing station already has an ingredient
            LOG_INFO("Can only have one ingredient on table.");
        } else {
            LOG_INFO("No item to work with.");
        }
    }
}

SETUP_DEFAULT_CALLBACKS(WorkStation)
