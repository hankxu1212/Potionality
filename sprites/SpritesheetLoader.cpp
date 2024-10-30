#include "SpritesheetLoader.h"
#include "../GL.hpp"
#include "../core/ResourceManager.h"
#include "../scene/Scene.hpp"
#include "../core/Time.hpp"
#include "../core/utils/Logger.hpp"

SpritesheetLoader::SpritesheetLoader(Shader& shader)
{
    this->shader = shader;
    initRenderData();
}

SpritesheetLoader::SpritesheetLoader(const std::string& shaderName, uint32_t width, uint32_t height) :
    dimX(width), dimY(height)
{
    Shader spriteShader = ResourceManager::GetShader(shaderName);
    shader = spriteShader;
    initRenderData();

    uv_step.x = 1.0 / dimX;
    uv_step.y = 1.0 / dimY;
}

SpritesheetLoader::~SpritesheetLoader()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpritesheetLoader::Update()
{
}

void SpritesheetLoader::DrawSprite(const std::string& name, float uv_x, float uv_y)
{
    shader.Use();

    // set transformations
    shader.SetMatrix4("model", GetTransform()->GetModel());
    shader.SetVector2f("textureDims", GetTransform()->size());
    
    // push animation variables
    shader.SetVector2f("uv_offset", glm::vec2(uv_x, uv_y));
    shader.SetVector2f("uv_step", uv_step);

    glActiveTexture(GL_TEXTURE0);
    ResourceManager::GetTexture(name).Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

static float vertices[] = {
    // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};

void SpritesheetLoader::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

template<>
void Scene::OnComponentAdded<SpritesheetLoader>(Entity& entity, SpritesheetLoader& component)
{
}

template<>
void Scene::OnComponentRemoved<SpritesheetLoader>(Entity& entity, SpritesheetLoader& component)
{
}