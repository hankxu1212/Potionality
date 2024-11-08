#include "ColliderManager.hpp"
#include "../scene/Entity.hpp"

ColliderManager::ColliderManager()
{
}

ColliderManager::~ColliderManager()
{
}

void ColliderManager::Update()
{
}

bool ColliderManager::CheckCollision(BoxCollider *collider)
{
	for (auto [uid, c] : m_AllColliders)
	{
		if (collider->Intersects(*c))
			return true;
	}
	return false;
}

void ColliderManager::Add(BoxCollider *collider)
{
    auto it = m_AllColliders.find(collider->entity->uuid());
	if (it != m_AllColliders.end()) {
		LOG_WARN("Added a collider multiple times. This should never happen.");
		return;
	}

	m_AllColliders[collider->entity->uuid()] = collider;
}

void ColliderManager::Remove(BoxCollider * collider)
{
    auto it = m_AllColliders.find(collider->entity->uuid());
	if (it != m_AllColliders.end()) {
		LOG_WARN("Tried to remove a collider that doesnt exist in all colliders list.");
		return;
	}

    m_AllColliders.erase(collider->entity->uuid());
}
