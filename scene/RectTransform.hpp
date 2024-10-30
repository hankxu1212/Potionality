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

    // Move Constructor
    RectTransform(RectTransform&& other) noexcept
        : position(std::move(other.position)), size(std::move(other.size)),
        rotation(other.rotation), depth(other.depth) {
        other.rotation = 0.0f;
        other.depth = 0.0f;
    }

	glm::mat4 GetModel() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

		model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

		return model;
	}
};
