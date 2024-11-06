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

	customerSprite = entity->GetComponent<SpritesheetRenderer>();
	m_CustomerState = State::Walk;
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
		//TextRenderer::Get()->RenderText("Can you help me make a potion?", GetTransform()->position().x, 1080 - GetTransform()->position().y);

		// walk towards middle
		GetTransform()->SetPositionX(GetTransform()->position().x - Time::DeltaTime * 100);

		m_InitialRequestTimer -= Time::DeltaTime;
		
	} else { m_CustomerState = State::Idle; }
	Customer::HandleAnimations();
}

void Customer::HandleAnimations()
{
	 if (m_CustomerState == State::Idle)
	 {
	 	customerSprite->SetLoopRegion(0, 0);
	 }
	 else if (m_CustomerState == State::Walk)
	 {
	 	customerSprite->SetLoopRegion(0, 3);
	 }
}

void Customer::Interact(InteractPayload* payload)
{
	payload->isInDialogue = true;
	DialogueUI::Instance->Enable();
	DialogueUI::Instance->StartConversation("Hi, can you give me sth?");
}

void Customer::InstantiateRequests(uint32_t numRequests)
{
	m_InitialRequestTimer = m_InitialRequestTimerMax;
}

SETUP_DEFAULT_CALLBACKS(Customer)