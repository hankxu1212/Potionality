#pragma once

#include <vector>

#include "Behaviour.hpp"

class SpriteLoader;

class Ingredient : public Behaviour
{
public:
	// list of ingredient instances
	inline static std::vector<Ingredient*> Instances;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Shutdown() override;

	const char* getClassName() const override { return "Ingredient"; }

private:
	float interactionDistance = 100;
};
