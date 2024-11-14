#include "SpriteRendererList.hpp"

#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"

void SpriteRendererList::Update()
{
	if (active && currentSpriteIndex != -1) 
	{
		assert(spriteLoader && currentSpriteIndex < sprites.size());
		spriteLoader->DrawSprite(sprites[currentSpriteIndex]);
	}
}

template<>
void Scene::OnComponentAdded<SpriteRendererList>(Entity& entity, SpriteRendererList& component)
{
	component.spriteLoader = entity.GetComponent<SpriteLoader>();
	assert(component.spriteLoader);
}

template<>
void Scene::OnComponentRemoved<SpriteRendererList>(Entity& entity, SpriteRendererList& component)
{
}