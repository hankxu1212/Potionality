#include "Entity.hpp"
#include "../core/utils/Bitmap.hpp"
#include "../core/utils/Logger.hpp"
#include "../core/Files.hpp"

#include <unordered_map>
#include <unordered_set>
#include <glm/glm.hpp>
#include <functional> // for std::hash

void* OffsetPointer(void* ptr, size_t offset) {
	return static_cast<void*>(static_cast<std::byte*>(ptr) + offset);
}

Entity::~Entity()
{
	RemoveAllComponents();
	m_Components.clear();
}

void Entity::Update()
{
    // update components
	for (auto& component : m_Components)
	{
		component->Update();
	}
}

void Entity::Destroy()
{
	m_Scene->Destroy(m_UUID);
}

// removes a component by a reference with some variadic templating
template<typename... C>
static void DestroyComponentClass(Entity& e)
{
	([&] { e.RemoveComponent<C>(); }(), ...);
}

template<typename... C>
static void DestroyComponentGroup(ComponentGroup<C...>, Entity& e)
{
	DestroyComponentClass<C...>(e);
}

void Entity::RemoveAllComponents() {
	DestroyComponentGroup(AllComponents{}, *this);
}
