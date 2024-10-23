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

void Entity::Update()
{
    // update components
	for (auto& component : m_Components)
	{
		component->Update();
	}
}