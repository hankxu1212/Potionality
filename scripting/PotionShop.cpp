#include "PotionShop.hpp"

#include "Customer.hpp"
#include "../scene/Entity.hpp"

#include "../math/Math.hpp"
#include "../scene/SceneManager.hpp"
#include "../PlayMode.hpp"

PotionShop::PotionShop()
{
	LoadAllPossibleCustomers();
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
	m_CurrentNumCustomers++;
}

void PotionShop::Remove(Customer* customer)
{
	auto it = m_Customers.find(customer->entity->uuid());
	if (it == m_Customers.end()) {
		LOG_WARN("Tried to remove a customer that was already removed or never added. This should never happen");
		return;
	}

	m_Customers.erase(customer->entity->uuid());
	m_CurrentNumCustomers--;
}

void PotionShop::SpawnNewCustomer()
{
	// spawns randomly on the right
	glm::vec2 randPos(1800, Math::RandomInt(300, 700));

	// change this lmao
	glm::vec2 randScale(Math::Random(100, 200), Math::Random(100, 200));

	Scene* scene = SceneManager::Get()->getScene();
	Entity* newCustomer = scene->Instantiate("CustomerInstance", randPos, randScale, 0, 2);

	// add some sprite stuff
	newCustomer->AddComponent<SpritesheetLoader>(true, SPRITESHEET_SHADER, 4,1);
	int customer = (uint32_t)Math::RandomInt(0, 3);
	if (customer == 0){
		newCustomer->AddComponent<SpritesheetRenderer>(true, "Customer1_walk");
	}
	else if (customer == 1){
		newCustomer->AddComponent<SpritesheetRenderer>(true, "Customer2_walk");
	}else {
		newCustomer->AddComponent<SpritesheetRenderer>(true, "Customer_Special_walk");
	}
	
	// add the customer script, instantiate a new request
	int customerType = Math::RandomInt(0, (int)AllCustomerTypes.size() - 1);
	Customer& cust = newCustomer->AddComponent<Customer>(true);
	cust.Initialize(AllCustomers[AllCustomerTypes[customerType]]);

	LOG_INFO_F("Spawned new customer:{}", AllCustomerTypes[customerType]);
}

void PotionShop::LoadAllPossibleCustomers()
{
	const std::string path = "../resources/scenes/Customers.json";
	sejp::value loaded = sejp::load(Files::Path(path));
	auto& map = loaded.as_object().value();
	const auto& customers = map.at("customers").as_array().value();

	for (const auto& cust : customers)
	{
		const auto& customer = cust.as_object().value();
		CustomerInfo info;
		info.Deserialize(customer);

		AllCustomers[info.m_Type] = info;

		AllCustomerTypes.emplace_back(info.m_Type);
	}
	LOG_INFO_F("Loaded {} customer types", AllCustomerTypes.size());
}

void PotionShop::Update()
{
	m_WaitCounter -= Time::DeltaTime;
	if (m_WaitCounter <= 0)
	{
		m_WaitCounter = Math::Random(15, 20);
		if (m_CurrentNumCustomers < MaxCustomers) {
			SpawnNewCustomer();
		}
	}
}
