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
	 	customerSprite->SetLoopRegion(2, 4);
	 }
	 else if (m_CustomerState == State::WalkForward)
	 {
	 	customerSprite->SetLoopRegion(0, 4);
	 }
	 else{
		customerSprite->SetLoopRegion(1, 4);
	 }
}

void Customer::Interact(InteractPayload* payload)
{
	bool setAsk = false;

	if (Player::Instance->getHeldObject())
	{
		Potion* potion = dynamic_cast<Potion*>(Player::Instance->getHeldObject());
		if (potion)
		{
			auto it = std::find(m_CustomerInfo.m_Request.potionsToMake.begin(), m_CustomerInfo.m_Request.potionsToMake.end(), potion->name);
			bool doubleReputation = Player::Instance->getDoubleReputation();
			if (it != m_CustomerInfo.m_Request.potionsToMake.end())
			{
				// Drop off item. Destroys it.
				potion->entity->Destroy();
				payload->isDestroyed = true;

				// walk back!
				HandleCorrectInteract(doubleReputation);
			}
			else
			{
				HandleIncorrectInteract(doubleReputation);
			}
		}
		else
			setAsk = true;
	}
	else
		setAsk = true;

	if (setAsk)
		currentInteractionString = m_CustomerInfo.m_MonologueOnAsk[Math::Random(0, m_CustomerInfo.m_MonologueOnAsk.size())];
	
	payload->isInDialogue = true;
	DialogueUI::Instance->Enable();
	DialogueUI::Instance->StartConversation(currentInteractionString);
}

void Customer::HandleCorrectInteract(bool doubleReputation)
{
	assert(m_CustomerInfo.m_MonologueOnRecieveSuccess.size() > 0);
	currentInteractionString = m_CustomerInfo.m_MonologueOnRecieveSuccess[Math::Random(0, m_CustomerInfo.m_MonologueOnRecieveSuccess.size())];

	m_MovementTimer = 6;
	walkDir = 1;
	destroyAfterMove = true;
	m_CustomerState = State::WalkBackward;

	if (doubleReputation) PotionShop::Get()->reputation += 2;
	else PotionShop::Get()->reputation++;
}

void Customer::HandleIncorrectInteract(bool doubleReputation)
{
	currentInteractionString = m_CustomerInfo.m_MonologueOnRecieveFailure[m_CustomerInfo.m_MonologueOnRecieveFailure.size() - patience];
	patience--;
	LOG_INFO(patience);
	if (patience==0)
	{
		// walk back!
		if (doubleReputation) PotionShop::Get()->reputation -= 2;
		else PotionShop::Get()->reputation--;
		m_MovementTimer = 6;
		walkDir = 1;
		destroyAfterMove = true;
		m_CustomerState = State::WalkBackward;
	}
}

void Customer::Initialize(CustomerInfo& info)
{
	m_MovementTimer = 5;
	patience = info.m_Patience;
	this->m_CustomerInfo = info;
}

SETUP_DEFAULT_CALLBACKS(Customer)