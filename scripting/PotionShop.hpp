#pragma once

#include "PotionCore.hpp"
#include "../core/Module.hpp"
#include "CustomerInfo.hpp"

class Customer;

class PotionShop : public Module::Registrar<PotionShop>
{
	inline static const bool Registered = Register(UpdateStage::Pre, DestroyStage::Post);
public:

	PotionShop();

	virtual ~PotionShop();

	void Update() override;

	void Add(Customer*);

	void Remove(Customer*);

	void SpawnNewCustomer();

private:
	std::unordered_map<size_t, Customer*> m_Customers;
	[[maybe_unused]] float reputation = 0;

	std::unordered_map<std::string, CustomerInfo> AllCustomers;

	float m_WaitCounter;

	void LoadAllPossibleCustomers();
};

