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
    bool xOverlap = std::abs((position.x + width / 2) - (other.position.x + other.width / 2)) <= (width / 2 + other.width / 2);
    // Check for overlap in y-axis
    bool yOverlap = std::abs((position.y + height / 2) - (other.position.y + other.height / 2)) <= (height / 2 + other.height / 2);
    //std::cout << "x pos" << position.x << " " << " other x pos " << other.position.x << "\n";
    //std::cout << "y pos" << position.y << " " << " other y pos " << other.position.y << "\n";
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
