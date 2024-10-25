#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Scene.hpp"
#include "Components.hpp"

#include "../sprites/SpriteLoader.h"
#include "RectTransform.hpp"

class Entity
{
public:
    void Update();

	template<typename... TArgs>
	Entity(Scene* scene, TArgs&... args) :
		m_Scene(scene), s_Transform(std::make_unique<RectTransform>(args...)) {
	}

	// template taking an lvalue
	template<typename... TArgs>
	Entity(Scene* scene, TArgs&&... args) :
		m_Scene(scene), s_Transform(std::make_unique<RectTransform>(args...)) {
	}

private:
    friend class Scene;

    bool draw; // is this entity draw or no

public: // entity components
	// Adds a component to an entity
	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		m_Components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		m_Components.back()->SetEntity(this);
		T& newComponent = *dynamic_cast<T*>(m_Components.back().get());
		m_Scene->OnComponentAdded(*this, newComponent);
		return newComponent;
	}

    template<typename T>
	T* GetComponent() const
	{
		for (const auto& component : m_Components)
		{
			if (dynamic_cast<T*>(component.get()))
				return (T*)component.get();
			else
				continue;
		}
		return nullptr;
	}

    void SetScene(Scene* newScene) { m_Scene = newScene; }
	Scene* scene() { return m_Scene; }
	RectTransform* transform() { return s_Transform.get(); }
	uint64_t uuid() { return uint64_t(m_UUID); }

private:
    Scene*									m_Scene = nullptr;
    std::vector<std::unique_ptr<Component>>	m_Components;
	std::unique_ptr<RectTransform>			s_Transform;
	UUID									m_UUID;
};