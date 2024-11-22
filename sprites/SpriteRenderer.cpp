#include "SpriteRenderer.h"

#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"

SpriteRenderer::SpriteRenderer(const std::string& spriteToDraw_) :
	spriteToDraw(spriteToDraw_)
{
}

void SpriteRenderer::Update()
{
	if (isActive) {
		assert(spriteLoader);
		spriteLoader->DrawSprite(spriteToDraw);
	}
}

template<>
void Scene::OnComponentAdded<SpriteRenderer>(Entity& entity, SpriteRenderer& component)
{
	component.spriteLoader = entity.GetComponent<SpriteLoader>();
}

template<>
void Scene::OnComponentRemoved<SpriteRenderer>(Entity& entity, SpriteRenderer& component)
{
}