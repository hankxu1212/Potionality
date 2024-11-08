#include "BoxCollider.hpp"
#include "../scene/Scene.hpp"
#include "ColliderManager.hpp"

BoxCollider::BoxCollider(glm::vec2 offsetFromCenter, glm::vec2 size) :
    offset(offsetFromCenter), width(size.x), height(size.y)
{
}

void BoxCollider::Update()
{
    position = GetTransform()->position() + offset;
}

bool BoxCollider::Intersects(const BoxCollider &other) const
{
    // Check for overlap in x-axis
    bool xOverlap = std::abs(position.x - other.position.x) <= (width / 2 + other.width / 2);
    // Check for overlap in y-axis
    bool yOverlap = std::abs(position.y - other.position.y) <= (height / 2 + other.height / 2);
    return xOverlap && yOverlap;
}

template<>																				\
void Scene::OnComponentAdded<BoxCollider>(Entity& entity, BoxCollider& component)
{
    ColliderManager::Get()->Add(&component);
}

template<>																				\
void Scene::OnComponentRemoved<BoxCollider>(Entity& entity, BoxCollider& component)
{
    ColliderManager::Get()->Remove(&component);
}