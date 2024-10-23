#include "SpriteRenderer.h"
#include "GL.hpp"
#include "core/ResourceManager.h"
#include "scene/Scene.hpp"

SpriteRenderer::SpriteRenderer(Shader& shader)
{
    shader = shader;
    initRenderData();
}

SpriteRenderer::SpriteRenderer(const std::string& shaderName)
{
    Shader spriteShader = ResourceManager::GetShader(shaderName);
    shader = spriteShader;
    initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::Update()
{
}

void SpriteRenderer::DrawSprite(const std::string& name, glm::vec3 color)
{
    // prepare transformations
    this->shader.Use();
    this->shader.SetMatrix4("model", GetTransform()->GetModel());
    this->shader.SetVector3f("spriteColor", color);

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

void SpriteRenderer::initRenderData()
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
void Scene::OnComponentAdded<SpriteRenderer>(Entity& entity, SpriteRenderer& component)
{
}

template<>
void Scene::OnComponentRemoved<SpriteRenderer>(Entity& entity, SpriteRenderer& component)
{
}