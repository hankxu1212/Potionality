#pragma once

#include "../../core/Module.hpp"

#include "InteractableObject.h"

class InteractableManager : public Module::Registrar<InteractableManager>
{
	inline static const bool Registered = Register(UpdateStage::Pre, DestroyStage::Post);
public:
	InteractableManager();

	virtual ~InteractableManager();

	void Update() override;

	void Add(InteractableObject*);

	void Remove(InteractableObject*);

	InteractableObject* GetClosestObject() const;

private:
	std::unordered_map<size_t, InteractableObject*> m_Interactables;
	InteractableObject* closestInteractableObject = nullptr;
	InteractableObject* prevInteractableObject = nullptr;

	const float interactionDistance = 100;
};

