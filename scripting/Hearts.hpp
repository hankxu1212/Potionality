#pragma once

#include "Behaviour.hpp"
#include "interactables/InteractableObject.h"

class Hearts : public Behaviour
{
public:
	void Awake() override;
	void Shutdown() override;
	void Update() override;

	void setPlayerBig(bool big) { playerIsBig = big; }

	const char* getClassName() const override { return "Hearts"; }

private:
    RectTransform* playerTransform;
    bool playerIsBig = false;

    bool idleFlag;
    float idleTime;
    const float idleTimeMax = 2.0f;

};