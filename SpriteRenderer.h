#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/Texture.h"
#include "core/Shader.h"
#include "scene/Component.hpp"

class SpriteRenderer : public Component
{
public:
    SpriteRenderer(Shader& shader);
    SpriteRenderer(const std::string& shaderName);
    ~SpriteRenderer();

    void Update() override;

    // Renders a defined quad textured with given sprite
    void DrawSprite(const std::string& name, glm::vec3 color = glm::vec3(1.0f));

private:
    Shader       shader;
    unsigned int quadVAO;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};