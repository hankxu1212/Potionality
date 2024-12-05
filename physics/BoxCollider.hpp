#pragma once

#include "../scene/Component.hpp"
#include <glm/glm.hpp>

class BoxCollider : public Component
{
public:
	BoxCollider(glm::vec2 offsetFromCenter, glm::vec2 size);
	
	void Update() override;

	bool Intersects(const BoxCollider& other) const;

	void setOffset(glm::vec2 newOffset) { offset = newOffset; }
	glm::vec2 getOffset() { return offset; }
	void setWidth(float w) { width = w; }
	void setHeight(float h) { height = h; }

private:
	friend class ColliderManager;
	glm::vec2 offset;
	glm::vec2 position;
	float width, height;
};