#pragma once

#include "../scene/Component.hpp"
#include <glm/glm.hpp>

class BoxCollider : public Component
{
public:
	BoxCollider(glm::vec2 offsetFromCenter, glm::vec2 size);
	
	void Update() override;

	bool Intersects(const BoxCollider& other) const;
private:
	glm::vec2 offset;
	glm::vec2 position;
	float width, height;
};