#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../core/Texture.h"
#include "../core/Shader.h"
#include "../scene/Component.hpp"

class SpritesheetLoader : public Component
{
public:
    SpritesheetLoader(Shader& shader);
    SpritesheetLoader(const std::string& shaderName, uint32_t width, uint32_t height);
    ~SpritesheetLoader();

    void Update() override;

    // Renders a defined quad textured with given sprite
    void DrawSprite(const std::string& name, float uv_x, float uv_y);

    [[nodiscard]] inline float getWidth() const { return dimX; }
    [[nodiscard]] inline float getHeight() const { return dimY; }

private:
    Shader       shader;
    unsigned int quadVAO;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();

    float dimX, dimY;
    glm::vec2 uv_step;
};