#include "SpritesheetRenderer.h"

#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"

SpritesheetRenderer::SpritesheetRenderer(const std::string& spriteToDraw_) :
	spriteToDraw(spriteToDraw_)
{
	prevTime = Time::Now;
}

void SpritesheetRenderer::Update()
{
	assert(spritesheetLoader);

	float timeDelta = Time::Now - prevTime;
    if (timeDelta > invFPS)
    {
        prevTime = Time::Now;
        uv_x += 1.0f;
        if (uv_x >= size_x) {
            uv_x = loop_x;
            uv_y += 1.0f;

            if (uv_y >= size_y)
                uv_y = loop_y;
        }
    }

	spritesheetLoader->DrawSprite(spriteToDraw, uv_x, uv_y);
}

void SpritesheetRenderer::SetLoopRegion(float row, float col)
{
    loop_y = row; 
    loop_x = col;
}

template <>
void Scene::OnComponentAdded<SpritesheetRenderer>(Entity &entity, SpritesheetRenderer &component)
{
	component.spritesheetLoader = entity.GetComponent<SpritesheetLoader>();
    component.size_x = component.spritesheetLoader->getWidth();
    component.size_y = component.spritesheetLoader->getHeight();
}

template<>
void Scene::OnComponentRemoved<SpritesheetRenderer>(Entity& entity, SpritesheetRenderer& component)
{
}