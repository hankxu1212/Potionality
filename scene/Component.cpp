#include "Component.hpp"
#include "../core/utils/Logger.hpp"
#include "RectTransform.hpp"
#include "Entity.hpp"

#define CHECK_ENTITY 	if (!entity) { LOG_WARN("Entity is null!"); return nullptr; }


void Component::SetEntity(Entity* thisEntity)
{
	entity = thisEntity;
}

RectTransform* Component::GetTransform()
{
	return entity->transform();
}

Scene* Component::GetScene()
{
	CHECK_ENTITY
	return entity->scene();
}
