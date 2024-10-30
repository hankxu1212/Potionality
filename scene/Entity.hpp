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
	~Entity();

    void Update();

	template<typename... TArgs>
	Entity(Scene* scene, TArgs&... args) :
		m_Scene(scene), s_Transform(std::make_unique<RectTransform>(args...)) {
	}

	template<typename... TArgs>
	Entity(Scene* scene, const std::string& name, TArgs&... args) :
		m_Scene(scene), m_Name(name), s_Transform(std::make_unique<RectTransform>(args...)) {
	}

	// template taking an lvalue
	template<typename... TArgs>
	Entity(Scene* scene, TArgs&&... args) :
		m_Scene(scene), s_Transform(std::make_unique<RectTransform>(args...)) {
	}

	template<typename... TArgs>
	Entity(Scene* scene, const std::string& name, TArgs&&... args) :
		m_Scene(scene), m_Name(name), s_Transform(std::make_unique<RectTransform>(args...)) {
	}

	// destroys self
	void Destroy();

private:
    friend class Scene;

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

	// Adds a component to an entity, toggling initial active
	template<typename T, typename... Args>
	T& AddComponent(bool active, Args&&... args)
	{
		m_Components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		m_Components.back()->SetEntity(this);
		T& newComponent = *dynamic_cast<T*>(m_Components.back().get());
		m_Scene->OnComponentAdded(*this, newComponent);
		newComponent.isActive = active;

		return newComponent;
	}

    template<typename T>
	T* GetComponent() const
	{
		for (const auto& component : m_Components)
		{
			if (dynamic_cast<T*>(component.get()))
				return (T*)component.get();
		}
		return nullptr;
	}

	// removes the first component by class type
	template<typename T>
	void RemoveComponent()
	{
		for (int i=0;i<m_Components.size();++i)
		{
			const auto& component = m_Components[i];

			if (dynamic_cast<T*>(component.get()))
			{
				T* comp = (T*)component.get();
				m_Scene->OnComponentRemoved(*this, *comp);
				m_Components.erase(m_Components.begin() + i);
			}
		}
	}

	void RemoveAllComponents();

public: // get and set
    void SetScene(Scene* newScene) { m_Scene = newScene; }
	Scene* scene() { return m_Scene; }

	RectTransform* transform() { return s_Transform.get(); }

	uint64_t uuid() { return uint64_t(m_UUID); }

	const std::string& name() { return m_Name; }

	bool isActive() const { return m_Active; }
	void SetActive(bool value) { m_Active = value; }

private:
    Scene*									m_Scene = nullptr;
	std::string								m_Name;
    std::vector<std::unique_ptr<Component>>	m_Components;
	std::unique_ptr<RectTransform>			s_Transform;
	UUID									m_UUID;
	bool									m_Active = true;
};