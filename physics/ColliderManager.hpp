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

	void Add(BoxCollider*);

	void Remove(BoxCollider*);

private:
    std::unordered_map<size_t, BoxCollider*> m_AllColliders;
};