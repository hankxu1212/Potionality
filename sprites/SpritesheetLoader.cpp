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

SpritesheetLoader::SpritesheetLoader(const std::string& shaderName)
{
    Shader spriteShader = ResourceManager::GetShader(shaderName);
    shader = spriteShader;
    initRenderData();
}

SpritesheetLoader::~SpritesheetLoader()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpritesheetLoader::Update()
{
    float timeDelta = Time::Now - prevTime;
    if (timeDelta > invFPS)
    {
        prevTime = Time::Now;
        uv_x += 1.0f;
        if (uv_x >= nx_frames) {
            uv_x = 0;
            uv_y += 1.0f;

            if (uv_y >= ny_frames)
                uv_y = 0;
        }
    }
}

void SpritesheetLoader::DrawSprite(const std::string& name, glm::vec3 color)
{
    shader.Use();

    // set transformations
    shader.SetMatrix4("model", GetTransform()->GetModel());
    shader.SetVector3f("spriteColor", color);
    
    // push animation variables
    shader.SetFloat("uv_x", uv_x);
    shader.SetFloat("uv_y", uv_y);
    shader.SetFloat("nx_frames", nx_frames);
    shader.SetFloat("ny_frames", ny_frames);

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

    // initialize animator
    prevTime = Time::Now;
}

template<>
void Scene::OnComponentAdded<SpritesheetLoader>(Entity& entity, SpritesheetLoader& component)
{
}

template<>
void Scene::OnComponentRemoved<SpritesheetLoader>(Entity& entity, SpritesheetLoader& component)
{
}