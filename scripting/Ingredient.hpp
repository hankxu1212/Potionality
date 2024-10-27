#pragma once

#include <vector>

#include "Behaviour.hpp"

class SpriteLoader;

class Ingredient : public Behaviour
{
public:
	// vector of ingredient instances
	inline static std::vector<Ingredient*> Instances;
	//inline static std::map<std::pair<float, float>, Ingredient*> Ingredients; // TODO: Better to use glm::vec2 as keys? Kind of a pain though

	void Awake() override;
	void Start() override;
	void Update() override;
	void Shutdown() override;

	const char* getClassName() const override { return "Ingredient"; }

	/*
	Entity* GetClosestIngredient(glm::vec2 playerPos) {
		float min_distance = -1;
		Entity* closest = nullptr;
		for (const auto& mapping : ingredient_locations) {
			std::pair<float, float> coord = mapping.first;
			float distance = glm::distance(playerPos, glm::vec2{coord.first, coord.second});
			if (distance < min_distance || min_distance == -1) {
				min_distance = distance;
				closest = mapping.second;
			}
		}
		return closest;
	}
	*/

private:
	float interactionDistance = 100;
};
