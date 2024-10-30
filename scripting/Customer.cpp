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

void Customer::Update()
{
	InteractableObject::Update(); // need to call baseclass explicitly

	if (m_InitialRequestTimer >= 0) {
		// display text
		RenderText("Can you help me make a potion?", GetTransform()->position().x, 1080 - GetTransform()->position().y);

		// walk towards middle
		GetTransform()->SetPositionX(GetTransform()->position().x - Time::DeltaTime * 100);

		m_InitialRequestTimer -= Time::DeltaTime;
	}
}

void Customer::Interact()
{
	LOG_INFO("Interacted with this customer!");
}

void Customer::InstantiateRequests(uint32_t numRequests)
{
	m_InitialRequestTimer = m_InitialRequestTimerMax;
}

SETUP_DEFAULT_CALLBACKS(Customer)