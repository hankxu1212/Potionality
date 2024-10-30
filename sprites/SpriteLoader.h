#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../core/Texture.h"
#include "../core/Shader.h"
#include "../scene/Component.hpp"

class SpriteLoader : public Component
{
public:
    SpriteLoader(Shader& shader);
    SpriteLoader(const std::string& shaderName);
    ~SpriteLoader();

    void Update() override;

    // Renders a defined quad textured with given sprite
    void DrawSprite(const std::string& name);

private:
    Shader       shader;
    unsigned int quadVAO;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};