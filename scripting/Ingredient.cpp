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
        GetTransform()->SetPosition(Player::Instance->GetTransform()->position().x + 64, Player::Instance->GetTransform()->position().y);
    }

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

void Ingredient::Interact(InteractPayload* payload)
{
	LOG_INFO("Interacted with this ingredient!");
    if (isHeld) {
        isHeld = false;
        GetTransform()->SetPosition(Player::Instance->GetTransform()->position().x + 192, Player::Instance->GetTransform()->position().y + 192);
    } else {
        isHeld = true;
        isStored = false;
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