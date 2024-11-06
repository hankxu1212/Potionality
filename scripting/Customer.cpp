#include "Customer.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "../PlayMode.hpp"
#include "PotionShop.hpp"
#include "../math/Math.hpp"

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
	if (Player::Instance->getHeldObject())
	{
		Potion* potion = dynamic_cast<Potion*>(Player::Instance->getHeldObject());
		if (potion)
		{
			auto it = std::find(info.m_Request.potionsToMake.begin(), info.m_Request.potionsToMake.end(), potion->name);
			if (it != info.m_Request.potionsToMake.end())
			{
				LOG_DEBUG(potion->name, Logger::CYAN, Logger::BOLD);
				
				// Drop off item. Destroys it.
				potion->entity->Destroy();

				assert(info.m_MonologueOnRecieveSuccess.size() > 0);
				currentInteractionString = info.m_MonologueOnRecieveSuccess[Math::Random(0, info.m_MonologueOnRecieveSuccess.size())];
			}
			else
			{
				assert(info.m_MonologueOnRecieveFailure.size() > 0);
				currentInteractionString = info.m_MonologueOnRecieveFailure[Math::Random(0, info.m_MonologueOnRecieveFailure.size())];
			}
		}
	}
	
	payload->isInDialogue = true;
	DialogueUI::Instance->Enable();
	DialogueUI::Instance->StartConversation(currentInteractionString);
}

void Customer::Initialize(CustomerInfo info)
{
	m_InitialRequestTimer = m_InitialRequestTimerMax;
	currentInteractionString = info.m_MonologueOnAsk[0];
	this->info = info;
}


SETUP_DEFAULT_CALLBACKS(Customer)