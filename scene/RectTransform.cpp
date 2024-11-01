#include "RectTransform.hpp"

// Default Constructor
RectTransform::RectTransform()
	: m_Position(0.0f, 0.0f), m_Size(1.0f, 1.0f), m_Rotation(0.0f), m_Depth(0.0f) {}

// Parameterized Constructor
RectTransform::RectTransform(const glm::vec2& pos, const glm::vec2& s, float rot, float d)
	: m_Position(pos), m_Size(s), m_Rotation(rot), m_Depth(d) {}

// Copy Constructor
RectTransform::RectTransform(const RectTransform& other)
	: m_Position(other.m_Position), m_Size(other.m_Size), m_Rotation(other.m_Rotation), m_Depth(other.m_Depth) {}


glm::vec2 RectTransform::position() const
{
	return m_Parent ? m_Parent->position() + m_Position : m_Position;
}

glm::vec2 RectTransform::size() const
{
	return m_Size;
}

float RectTransform::rotation() const
{
	return m_Parent ? m_Parent->rotation() + m_Rotation : m_Rotation;
}

void RectTransform::SetPositionX(float x)
{
	m_Position.x = x;
	isDirty = true;
}

void RectTransform::SetPositionY(float y)
{
	m_Position.y = y;
	isDirty = true;
}

void RectTransform::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
	isDirty = true;
}

glm::mat4& RectTransform::GetModel()
{
	if (isDirty) {
		model = glm::translate(glm::mat4(1.0f), glm::vec3(position(), m_Depth));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		model = glm::translate(model, glm::vec3(0.5f * m_Size.x, 0.5f * m_Size.y, 0.0f)); // move origin of rotation to center of quad
		model = glm::rotate(model, glm::radians(rotation()), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
		model = glm::translate(model, glm::vec3(-0.5f * m_Size.x, -0.5f * m_Size.y, 0.0f)); // move origin back

		model = glm::scale(model, glm::vec3(m_Size, 1.0f)); // last scale
		isDirty = false;
	}
	return model;
}

void RectTransform::Translate(const glm::vec2& offset)
{
	m_Position += offset;
	isDirty = true;
}