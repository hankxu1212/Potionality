#pragma once

#include "PotionCore.hpp"
#include "../core/Module.hpp"

class Customer;

class PotionShop : public Module::Registrar<PotionShop>
{
	inline static const bool Registered = Register(UpdateStage::Pre, DestroyStage::Normal);
public:

	PotionShop();

	virtual ~PotionShop();

	void Update() override;

	void Add(Customer*);

	void Remove(Customer*);

	void SpawnNewCustomer();

private:
	std::unordered_map<size_t, Customer*> m_Customers;
	float reputation = 0;

	float m_WaitCounter;
};

