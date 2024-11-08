#pragma once
#include "../core/Module.hpp"
#include "BoxCollider.hpp"

class ColliderManager : public Module::Registrar<ColliderManager>
{
	inline static const bool Registered = Register(UpdateStage::Never, DestroyStage::Normal);
public:
	ColliderManager();

	virtual ~ColliderManager();

    void Update() override;

    // check if the collider is in collision with anything else
    bool CheckCollision(BoxCollider* collider);

	// checks if the collider will be in collision given a next step positional offset
	bool CheckCollisionFuture(BoxCollider* collider, glm::vec2 positionalOffset);

	void Add(BoxCollider*);

	void Remove(BoxCollider*);

private:
	std::vector<BoxCollider*> m_AllColliders;
};