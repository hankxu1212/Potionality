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

	customerSprite = entity->children()[0]->GetComponent<SpritesheetRenderer>();
	m_CustomerState = State::WalkForward;
}

void Customer::Shutdown()
{
	InteractableObject::Shutdown(); // need to call baseclass explicitly
	PotionShop::Get()->Remove(this);
}

void Customer::Update()
{
	InteractableObject::Update(); // need to call baseclass explicitly
	if (m_MovementTimer >= 0) {
		// walk towards middle
		GetTransform()->SetPositionX(GetTransform()->position().x + walkDir * Time::DeltaTime * walkSpeedPerSecond);

		m_MovementTimer -= Time::DeltaTime;
		
	} else 
	{ 
		if (destroyAfterMove) {
			entity->Destroy();
			return;
		}
		m_CustomerState = State::Idle; 
	}


	Customer::HandleAnimations();
}

void Customer::HandleAnimations()
{
	 if (m_CustomerState == State::Idle)
	 {
	 	customerSprite->SetLoopRegion(2, 3);
	 }
	 else if (m_CustomerState == State::WalkForward)
	 {
	 	customerSprite->SetLoopRegion(0, 3);
	 }
	 else{
		customerSprite->SetLoopRegion(1, 3);
	 }
}

void Customer::Interact(InteractPayload* payload)
{
	if (Player::Instance->getHeldObject())
	{
		Potion* potion = dynamic_cast<Potion*>(Player::Instance->getHeldObject());
		if (potion)
		{
			auto it = std::find(m_CustomerInfo.m_Request.potionsToMake.begin(), m_CustomerInfo.m_Request.potionsToMake.end(), potion->name);
			if (it != m_CustomerInfo.m_Request.potionsToMake.end())
			{
				LOG_DEBUG(potion->name, Logger::CYAN, Logger::BOLD);
				
				// Drop off item. Destroys it.
				potion->entity->Destroy();
				payload->isDestroyed = true;

				assert(m_CustomerInfo.m_MonologueOnRecieveSuccess.size() > 0);
				currentInteractionString = m_CustomerInfo.m_MonologueOnRecieveSuccess[Math::Random(0, m_CustomerInfo.m_MonologueOnRecieveSuccess.size())];

				// walk back!
				m_MovementTimer = 6;
				walkDir = 1;
				destroyAfterMove = true;
				m_CustomerState = State::WalkBackward;

				PotionShop::Get()->reputation++;
			}
			else
			{
				assert(m_CustomerInfo.m_MonologueOnRecieveFailure.size() > 0);
				currentInteractionString = m_CustomerInfo.m_MonologueOnRecieveFailure[Math::Random(0, m_CustomerInfo.m_MonologueOnRecieveFailure.size())];
				patience --;
				if (patience==0){
					// walk back!
					PotionShop::Get()->reputation--;
					m_MovementTimer = 6;
					walkDir = 1;
					m_CustomerState = State::WalkBackward;
				}
			}
		}
	}
	
	payload->isInDialogue = true;
	DialogueUI::Instance->Enable();
	DialogueUI::Instance->StartConversation(currentInteractionString);
}

void Customer::Initialize(CustomerInfo info)
{
	m_MovementTimer = 5;
	patience = this->m_CustomerInfo.m_Patience;
	this->m_CustomerInfo = info;
	currentInteractionString = m_CustomerInfo.m_MonologueOnAsk[0];
}

SETUP_DEFAULT_CALLBACKS(Customer)