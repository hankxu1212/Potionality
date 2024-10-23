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
    SpritesheetLoader(const std::string& shaderName);
    ~SpritesheetLoader();

    void Update() override;

    // Renders a defined quad textured with given sprite
    void DrawSprite(const std::string& name, glm::vec3 color = glm::vec3(1.0f));

private:
    Shader       shader;
    unsigned int quadVAO;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();

    float nx_frames = 5.0f;
    float ny_frames = 5.0f;
    float uv_x = 0;
    float uv_y = 2;
    float invFPS = 1 / 2;
    float prevTime;
};