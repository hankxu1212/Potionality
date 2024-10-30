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

	inline void Activate() { active = true; }
	inline void Deactivate() { active = false; }
	inline bool GetActive() { return active; }

	const char* getClassName() const override { return "Ingredient"; }

private:
	float interactionDistance = 100;
	bool active = true; // TODO: Move active and relevant functions to Behaviour? Might need for several scripts
};
