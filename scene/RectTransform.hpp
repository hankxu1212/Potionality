#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RectTransform
{
public:
	glm::vec2 position;
	glm::vec2 size;
	float rotation;

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
