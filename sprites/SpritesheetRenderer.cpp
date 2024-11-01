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
        if (uv_x >= loop_x) {
            uv_x = 0;
            // uv_y += 1.0f;
            uv_y = loop_y; // TODO: modify this

            // if (uv_y >= loop_y)
                // uv_y = 0;
        }
    }

	spritesheetLoader->DrawSprite(spriteToDraw, uv_x, uv_y);
}

void SpritesheetRenderer::SetLoopRegion(float row, float col)
{
    if (prev_row == row && prev_col == col)
        return;

    prev_row = row;
    prev_col = col;

    loop_y = row;
    loop_x = col;

    uv_x = 0;
    uv_y = loop_y;
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