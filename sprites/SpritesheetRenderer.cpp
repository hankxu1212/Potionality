#include "SpritesheetRenderer.h"

#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"

SpritesheetRenderer::SpritesheetRenderer(const std::string& spriteToDraw_) :
	spriteToDraw(spriteToDraw_)
{
}

void SpritesheetRenderer::Update()
{
	assert(spritesheetLoader);
	spritesheetLoader->DrawSprite(spriteToDraw);
}

template<>
void Scene::OnComponentAdded<SpritesheetRenderer>(Entity& entity, SpritesheetRenderer& component)
{
	component.spritesheetLoader = entity.GetComponent<SpritesheetLoader>();
}

template<>
void Scene::OnComponentRemoved<SpritesheetRenderer>(Entity& entity, SpritesheetRenderer& component)
{
}