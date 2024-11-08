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
	for (BoxCollider* c : m_AllColliders)
	{
		if (c->GetEntityID() == collider->GetEntityID())
			continue;

		if (collider->Intersects(*c))
			return true;
	}
	return false;
}

bool ColliderManager::CheckCollisionFuture(BoxCollider* collider, glm::vec2 positionalOffset)
{
	collider->position += positionalOffset;

	bool res = CheckCollision(collider);

	collider->position -= positionalOffset;

	return res;
}

void ColliderManager::Add(BoxCollider *collider)
{
	assert(collider);
    auto it = std::find(m_AllColliders.begin(), m_AllColliders.end(), collider);
	if (it != m_AllColliders.end()) {
		LOG_WARN("Added a collider multiple times. This should never happen.");
		return;
	}

	m_AllColliders.emplace_back(collider);
}

void ColliderManager::Remove(BoxCollider * collider)
{
	assert(collider);
	auto it = std::find(m_AllColliders.begin(), m_AllColliders.end(), collider);
	if (it == m_AllColliders.end()) {
		LOG_WARN("Tried to remove a collider that doesnt exist in all colliders list.");
		return;
	}

	m_AllColliders.erase(it);
}
