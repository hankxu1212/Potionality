#include "PotionShop.hpp"

#include "Customer.hpp"
#include "../scene/Entity.hpp"

#include "../math/Math.hpp"
#include "../scene/SceneManager.hpp"
#include "../PlayMode.hpp"

PotionShop::PotionShop()
{
}

PotionShop::~PotionShop()
{
}

void PotionShop::Add(Customer* customer)
{
	auto it = m_Customers.find(customer->entity->uuid());
	if (it != m_Customers.end()) {
		LOG_WARN("Added a customer multiple times. This should never happen.");
		return;
	}

	m_Customers[customer->entity->uuid()] = customer;
}

void PotionShop::Remove(Customer* customer)
{
	auto it = m_Customers.find(customer->entity->uuid());
	if (it == m_Customers.end()) {
		LOG_WARN("Tried to remove a customer that was already removed or never added. This should never happen");
		return;
	}

	m_Customers.erase(customer->entity->uuid());
}

void PotionShop::SpawnNewCustomer()
{
	LOG_INFO("Spawning new customer...");

	// spawns randomly on the right
	glm::vec2 randPos(1800, Math::RandomInt(300, 700));

	// change this lmao
	glm::vec2 randScale(Math::Random(100, 200), Math::Random(100, 200));

	Scene* scene = SceneManager::Get()->getScene();
	Entity* newCustomer = scene->Instantiate("customer", randPos, randScale, 0, 2);

	// add some sprite stuff
	newCustomer->AddComponent<SpriteLoader>(true, LIT_SPRT_SHADER);
	newCustomer->AddComponent<SpriteRenderer>(true, "Guy");
	
	// add the customer script, instantiate a new request
	Customer& cust = newCustomer->AddComponent<Customer>(true);
	cust.InstantiateRequests((uint32_t)Math::RandomInt(1, 2));
}

void PotionShop::Update()
{
	m_WaitCounter -= Time::DeltaTime;
	if (m_WaitCounter <= 0)
	{
		m_WaitCounter = Math::Random(5, 10); // TODO: change this
		SpawnNewCustomer();
	}
}
