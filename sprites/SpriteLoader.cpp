#include "SpriteLoader.h"
#include "../GL.hpp"
#include "../core/ResourceManager.h"
#include "../scene/Scene.hpp"
#include "../core/utils/Logger.hpp"

SpriteLoader::SpriteLoader(Shader& shader)
{
    this->shader = shader;
    initRenderData();
}

SpriteLoader::SpriteLoader(const std::string& shaderName)
{
    Shader spriteShader = ResourceManager::GetShader(shaderName);
    shader = spriteShader;
    initRenderData();
}

SpriteLoader::~SpriteLoader()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteLoader::Update()
{
}

void SpriteLoader::DrawSprite(const std::string& name)
{
    // prepare transformations
    shader.Use();
    shader.SetMatrix4("model", GetTransform()->GetModel());
    shader.SetVector2f("textureDims", GetTransform()->size());

    glActiveTexture(GL_TEXTURE0);
    ResourceManager::GetTexture(name).Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteLoader::DrawSprite(const std::string& name, glm::vec2 position, glm::vec2 size)
{
    // prepare transformations
    shader.Use();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 1));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    shader.SetMatrix4("model", model);
    shader.SetVector2f("textureDims", size);

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

void SpriteLoader::initRenderData()
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
void Scene::OnComponentAdded<SpriteLoader>(Entity& entity, SpriteLoader& component)
{
}

template<>
void Scene::OnComponentRemoved<SpriteLoader>(Entity& entity, SpriteLoader& component)
{
}