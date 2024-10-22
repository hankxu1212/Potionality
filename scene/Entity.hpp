#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Scene.hpp"
#include "Components.hpp"

#include "../SpriteRenderer.h"

class Entity
{
public:
    Entity() = default;

    void Update();

    // loads an image, and split it into a bunch of 8x8 sprites
    void Load(const std::string& relativePath);

    glm::vec2 position;

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

private:
    Scene*									m_Scene = nullptr;
    std::vector<std::unique_ptr<Component>>	m_Components;
};