#include "Ingredient.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "../PlayMode.hpp"
#include "PotionShop.hpp"

void Ingredient::Awake()
{
	InteractableObject::Awake(); // need to call baseclass explicitly
    isHoldable = true;
}

void Ingredient::Shutdown()
{
	InteractableObject::Shutdown(); // need to call baseclass explicitly
}

void Ingredient::Update()
{
	InteractableObject::Update(); // need to call baseclass explicitly
    if (isHeld) {
        GetTransform()->SetPosition(Player::Instance->GetTransform()->position().x + 20, Player::Instance->GetTransform()->position().y - 25);
    }
}

void Ingredient::Interact(InteractPayload* payload)
{
	LOG_INFO("Interacted with this ingredient!");
    if (isHeld) {
        isHeld = false;
        GetTransform()->SetPosition(Player::Instance->GetTransform()->position().x + 60, Player::Instance->GetTransform()->position().y + 70);
    } else {
        isHeld = true;
    }
}

SETUP_DEFAULT_CALLBACKS(Ingredient)