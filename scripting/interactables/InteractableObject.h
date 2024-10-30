#pragma once

#include "../Behaviour.hpp"

class InteractableObject : public Behaviour
{
public:
	virtual void Interact() {}

	void SetActive();

	void SetInactive();

protected:
	void Awake() override;
	void Shutdown() override;
	void Update() override;

	bool isCurrentInteractable = false;
};

