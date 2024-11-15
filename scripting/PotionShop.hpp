#pragma once

#include "PotionCore.hpp"
#include "../core/Module.hpp"
#include "CustomerInfo.hpp"
#include <array>

class Customer;

#define MAX_CUSTOMERS 5

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

	int reputation = 0;
	
private:
	std::unordered_map<size_t, Customer*> m_Customers;

	std::unordered_map<std::string, CustomerInfo> AllCustomers;
	std::vector<std::string> AllCustomerTypes;

	float m_WaitCounter;
	std::array<bool, MAX_CUSTOMERS> m_AvailableSlots { true,true,true,true,true };
	const std::array<uint32_t, MAX_CUSTOMERS> m_CustomerSlots { 300, 400, 500, 600, 700 };
	uint32_t m_CurrentNumCustomers = 0;
	void LoadAllPossibleCustomers();

	void SpawnUI();
};

