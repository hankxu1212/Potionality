#include "Ingredient.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "../PlayMode.hpp"
#include "PotionShop.hpp"
#include "../sprites/SpriteRenderer.h"

void Ingredient::Awake()
{
	InteractableObject::Awake(); // need to call baseclass explicitly
    isHoldable = true;
    ingredient = new Ingredient_T;

    ingredient->name = entity->name();

    ingredient->executableActions.resize(4);
    ingredient->executableActions[0] = Action::Smash;
    ingredient->executableActions[1] = Action::Eat;
    ingredient->executableActions[2] = Action::Brew;
    ingredient->executableActions[3] = Action::Cut;

    ActionState smashes;
    ActionState eats;
    ActionState brews;
    ActionState cuts;

    ingredient->actionStates[Action::Smash] = smashes;
    ingredient->actionStates[Action::Eat] = eats;
    ingredient->actionStates[Action::Brew] = brews;
    ingredient->actionStates[Action::Cut] = cuts;
}

void Ingredient::Shutdown()
{
    delete ingredient;
	InteractableObject::Shutdown(); // need to call baseclass explicitly
}

void Ingredient::Update()
{
	InteractableObject::Update(); // need to call baseclass explicitly
    if (isHeld) {
        GetTransform()->SetDepth(1.0);
        GetTransform()->SetPosition(Player::Instance->GetTransform()->position().x + 64, Player::Instance->GetTransform()->position().y);
    }
    if (isStored){ // if on workstation, it should be on top of it
        GetTransform()->SetDepth(4.0) ;//GetComponent<SpriteRenderer>();
    }
    //  else {} //set depth to 1 again }

    // If ingredient action states have progressed sufficiently, update ingredient name and action states
    if (ingredient->name.compare("FlowerIngredient") == 0) {
        if (ingredient->actionStates[Action::Cut].progress > 0) {
            ingredient->name = "FlowerCutIngredient";
            ingredient->actionStates[Action::Smash].progress = 0;
            ingredient->actionStates[Action::Eat].progress = 0;
            ingredient->actionStates[Action::Brew].progress = 0;
            ingredient->actionStates[Action::Cut].progress = 0;

            entity->RemoveComponent<SpriteRenderer>();
            entity->AddComponent<SpriteRenderer>(true, "Guy");
        }
    } else if (ingredient->name.compare("RoseIngredient") == 0) {
        if (ingredient->actionStates[Action::Cut].progress > 0) {
            ingredient->name = "RoseCutIngredient";
            ingredient->actionStates[Action::Smash].progress = 0;
            ingredient->actionStates[Action::Eat].progress = 0;
            ingredient->actionStates[Action::Brew].progress = 0;
            ingredient->actionStates[Action::Cut].progress = 0;

            entity->RemoveComponent<SpriteRenderer>();
            entity->AddComponent<SpriteRenderer>(true, "rose_cut");
            // TODO update when rose cut sprite is finished
        }
    } else if (ingredient->name.compare("LeafIngredient") == 0) {
        if (ingredient->actionStates[Action::Cut].progress > 0) {
            ingredient->name = "LeafCutIngredient";
            ingredient->actionStates[Action::Smash].progress = 0;
            ingredient->actionStates[Action::Eat].progress = 0;
            ingredient->actionStates[Action::Brew].progress = 0;
            ingredient->actionStates[Action::Cut].progress = 0;

            entity->RemoveComponent<SpriteRenderer>();
            entity->AddComponent<SpriteRenderer>(true, "Guy");
            // TODO update when leaf cut sprite  -- if any is finished
        }
    } else if (ingredient->name.compare("MushroomIngredient") == 0) {
        if (ingredient->actionStates[Action::Cut].progress > 0) {
            ingredient->name = "MushroomCutIngredient";
            ingredient->actionStates[Action::Smash].progress = 0;
            ingredient->actionStates[Action::Eat].progress = 0;
            ingredient->actionStates[Action::Brew].progress = 0;
            ingredient->actionStates[Action::Cut].progress = 0;

            entity->RemoveComponent<SpriteRenderer>();
            entity->AddComponent<SpriteRenderer>(true, "mushroom_cut");
        }
    } else if (ingredient->name.compare("PurpleQuartzIngredient") == 0) {
        if (ingredient->actionStates[Action::Smash].progress > 0) {
            ingredient->name = "PurpleQuartzSmashedIngredient";
            ingredient->actionStates[Action::Smash].progress = 0;
            ingredient->actionStates[Action::Eat].progress = 0;
            ingredient->actionStates[Action::Brew].progress = 0;
            ingredient->actionStates[Action::Cut].progress = 0;

            entity->RemoveComponent<SpriteRenderer>();
            entity->AddComponent<SpriteRenderer>(true, "purple_quartz_smashed");
        }
    } else if (ingredient->name.compare("WhiteQuartzIngredient") == 0) {
        if (ingredient->actionStates[Action::Smash].progress > 0) {
            ingredient->name = "WhiteQuartzSmashedIngredient";
            ingredient->actionStates[Action::Smash].progress = 0;
            ingredient->actionStates[Action::Eat].progress = 0;
            ingredient->actionStates[Action::Brew].progress = 0;
            ingredient->actionStates[Action::Cut].progress = 0;

            entity->RemoveComponent<SpriteRenderer>();
            entity->AddComponent<SpriteRenderer>(true, "white_quartz_smashed");
        }
    }
}

void Ingredient::duplicateIngredient() {
    glm::vec2 newPos(GetTransform()->position().x, GetTransform()->position().y);
    Entity* newIngredient = SceneManager::Get()->getScene()->Instantiate(ingredient->name, newPos, glm::vec2{64, 64}, 0, 1);
    newIngredient->AddComponent<SpriteLoader>(true, "sprtShader");
    newIngredient->AddComponent<SpriteRenderer>(true, entity->GetComponent<SpriteRenderer>()->spriteToDraw);
    newIngredient->AddComponent<Ingredient>(true);
}

void Ingredient::Interact(InteractPayload* payload)
{
	LOG_INFO("Interacted with this ingredient!");
    if (isHeld) {
        isHeld = false;
        GetTransform()->SetPosition(Player::Instance->GetTransform()->position().x + 192, Player::Instance->GetTransform()->position().y + 192);
    } else if (Player::Instance->getHeldObject() == nullptr) {
        isHeld = true;
        isStored = false;

        // If ingredient is still in starting position, duplicate
        bool flowerOnShelf = ingredient->name.compare("FlowerIngredient") == 0 && 
            GetTransform()->position().x == 160 && GetTransform()->position().y == 236;
        bool mushroomOnShelf = ingredient->name.compare("MushroomIngredient") == 0 && 
            GetTransform()->position().x == 280 && GetTransform()->position().y == 236;
        bool roseOnShelf = ingredient->name.compare("RoseIngredient") == 0 && 
            GetTransform()->position().x == 160 && GetTransform()->position().y == 186;
        bool leafOnShelf = ingredient->name.compare("LeafIngredient") == 0 && 
            GetTransform()->position().x == 280 && GetTransform()->position().y == 186;
        bool purpleQuartzOnShelf = ingredient->name.compare("PurpleQuartzIngredient") == 0 && 
            GetTransform()->position().x == 160 && GetTransform()->position().y == 136;
        bool whiteQuartzOnShelf = ingredient->name.compare("WhiteQuartzIngredient") == 0 && 
            GetTransform()->position().x == 280 && GetTransform()->position().y == 136;
        if (flowerOnShelf || mushroomOnShelf || purpleQuartzOnShelf || whiteQuartzOnShelf || roseOnShelf || leafOnShelf) {
            duplicateIngredient();
        }
    }
}

void Ingredient::process(Action action) {
    switch (action) {
        case Action::None:
            LOG_INFO("Invalid ingredient processing action.");
            break;
        case Action::Smash:
            LOG_INFO("Smashed ingredient.");
            ingredient->actionStates[Action::Smash].progress++;
            break;
        case Action::Eat:
            LOG_INFO("Ate ingredient.");
            ingredient->actionStates[Action::Eat].progress++;
            break;
        case Action::Brew: // Note: Unnecessary?
            LOG_INFO("Brewed ingredient.");
            ingredient->actionStates[Action::Brew].progress++;
            break;
        case Action::Cut:
            ingredient->actionStates[Action::Cut].progress++;
            LOG_INFO("Cut ingredient.");
            break;
    }
}

SETUP_DEFAULT_CALLBACKS(Ingredient)
