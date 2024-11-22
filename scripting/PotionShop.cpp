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

	m_AvailableSlots[customer->slot] = true;

	m_Customers.erase(customer->entity->uuid());
	m_CurrentNumCustomers--;
}

void PotionShop::SpawnNewCustomer()
{
	// spawns randomly on the right
	int availableSlot = -1;
	for (int i=0;i<MAX_CUSTOMERS;++i)
	{
		if (m_AvailableSlots[i])
		{
			availableSlot = i;
			m_AvailableSlots[i] = false;
			break;
		}
	}
	if (availableSlot == -1){
		LOG_WARN("no available slots!");
		return;
	}

	glm::vec2 randPos(Math::Random(1800, 2000), m_CustomerSlots[availableSlot]);

	float scale = Math::Random(200, 256);
	glm::vec2 randScale(scale, scale);

	Scene* scene = SceneManager::Get()->getScene();
	Entity* newCustomer = scene->Instantiate("CustomerInstance", randPos, randScale, 0.0, 2+ .1*availableSlot);

	// add some sprite stuff
	Entity * offsetSprite = newCustomer->AddChild("sprite", {100,0}, randScale, 0.0f ,2+ .1*availableSlot);
    // offsetSprite->AddComponent<SpriteLoader>(SPRITE_SHADER);

	// newCustomer->AddComponent<SpritesheetLoader>(true, SPRITESHEET_SHADER, 4,3);
	offsetSprite->AddComponent<SpritesheetLoader>(true, SPRITESHEET_SHADER, 4,3);
	int customer = (uint32_t)Math::RandomInt(0, 3);
	if (customer == 0){
		offsetSprite->AddComponent<SpritesheetRenderer>(true, "Customer1_walk");
		// newCustomer->AddComponent<SpritesheetRenderer>(true, "Customer1_walk");
	}
	else if (customer == 1){
		offsetSprite->AddComponent<SpritesheetRenderer>(true, "Customer2_walk");
		// newCustomer->AddComponent<SpritesheetRenderer>(true, "Customer2_walk");
	// }else if (customer == 2){
	// 	newCustomer->AddComponent<SpritesheetRenderer>(true, "Customer3_walk");
	}else {
		offsetSprite->AddComponent<SpritesheetRenderer>(true, "Customer_Special_walk");
		// newCustomer->AddComponent<SpritesheetRenderer>(true, "Customer_Special_walk");
	}
	
	// add the customer script, instantiate a new request
	int customerType = Math::RandomInt(0, (int)AllCustomerTypes.size() - 1);
	Customer& cust = newCustomer->AddComponent<Customer>(true);
	cust.Initialize(AllCustomers[AllCustomerTypes[customerType]]);
	cust.slot = (uint32_t)availableSlot;

	LOG_INFO_F("Spawned new customer:{}, slot {}", AllCustomerTypes[customerType], cust.slot);
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

void PotionShop::SpawnUI()
{
	
}

void PotionShop::Update()
{
	m_WaitCounter -= Time::DeltaTime;
	if (m_WaitCounter <= 0)
	{
		m_WaitCounter = Math::Random(1, 2);
		if (m_CurrentNumCustomers < MAX_CUSTOMERS) {
			SpawnNewCustomer();
		}
	}

	std::string repStr = ": " + std::to_string(reputation);
	TextRenderer::Get()->RenderText(repStr, 1680, 1000, 0.002);
}
