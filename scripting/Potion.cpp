#include "Potion.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "../PlayMode.hpp"
#include "PotionShop.hpp"

Potion::Potion(const std::string &name_) :
    name(name_)
{
}

void Potion::Awake()
{
	InteractableObject::Awake(); // need to call baseclass explicitly
    isHoldable = true;
}

void Potion::Shutdown()
{
	InteractableObject::Shutdown(); // need to call baseclass explicitly
}

void Potion::Update()
{
	InteractableObject::Update(); // need to call baseclass explicitly
    if (isHeld) {
        GetTransform()->SetPosition(Player::Instance->GetTransform()->position().x + 20, Player::Instance->GetTransform()->position().y - 25);
    }
}

void Potion::Interact(InteractPayload* payload)
{
	LOG_INFO("Interacted with this Potion!");
    if (isHeld) {
        isHeld = false;
        GetTransform()->SetPosition(Player::Instance->GetTransform()->position().x + 60, Player::Instance->GetTransform()->position().y + 70);
    } else {
        isHeld = true;
    }
}

SETUP_DEFAULT_CALLBACKS(Potion)