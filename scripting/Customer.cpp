#include "Customer.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "../PlayMode.hpp"
#include "PotionShop.hpp"

void Customer::Awake()
{
	InteractableObject::Awake(); // need to call baseclass explicitly
	PotionShop::Get()->Add(this);
}

void Customer::Shutdown()
{
	InteractableObject::Shutdown(); // need to call baseclass explicitly
	PotionShop::Get()->Remove(this);
}

void Customer::Interact()
{
	LOG_INFO("Inetracted with this customer!");
}

SETUP_DEFAULT_CALLBACKS(Customer)