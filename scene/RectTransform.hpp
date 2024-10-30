#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RectTransform
{
public:
	glm::vec2 position;
	glm::vec2 size;
	float rotation;
	float depth;

    // Default Constructor
    RectTransform()
        : position(0.0f, 0.0f), size(1.0f, 1.0f), rotation(0.0f), depth(0.0f) {}

    // Parameterized Constructor
    RectTransform(const glm::vec2& pos, const glm::vec2& s, float rot = 0.0f, float d = 0.0f)
        : position(pos), size(s), rotation(rot), depth(d) {}

    // Constructor with individual float values for position and size
    RectTransform(float posX, float posY, float width, float height, float rot = 0.0f, float d = 0.0f)
        : position(posX, posY), size(width, height), rotation(rot), depth(d) {}

    // Copy Constructor
    RectTransform(const RectTransform& other)
        : position(other.position), size(other.size), rotation(other.rotation), depth(other.depth) {}

    glm::mat4 GetModel() const;
};
