#include "PotionShop.hpp"

#include "Customer.hpp"
#include "../scene/Entity.hpp"

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

void PotionShop::Update()
{
}
