#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RectTransform
{
public:
    RectTransform();
    RectTransform(const glm::vec2& pos, const glm::vec2& s, float rot = 0.0f, float d = 0.0f);
    RectTransform(const RectTransform& other);

    // this is explicitly done to avoid extra matrix multiplications and dirty flag caching
    // thats why the variables are made private
    glm::vec2 position() const;
    glm::vec2 size() const;
    float rotation() const;

    void SetPositionX(float x);
    void SetPositionY(float y);
    void SetPosition(float x, float y);
    // TODO: add more setters here as you wish, but DO NOT move the private variables to public, or caching breaks

    glm::mat4& GetModel();

    void SetParent(RectTransform* newParent) { m_Parent = newParent; }

    void Translate(const glm::vec2& offset);

    bool IsDirty() const { return isDirty; }
    void SetDirty() { isDirty = true; }

private: // DO NOT move the private variables to public, or caching breaks
    RectTransform* m_Parent = nullptr;
	glm::vec2 m_Position;
	glm::vec2 m_Size;
	float m_Rotation;
	float m_Depth;

    bool isDirty = true;
    glm::mat4 model;
};
