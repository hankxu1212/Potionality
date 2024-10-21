#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Scene.hpp"
#include "Components.hpp"
#include "../PPU466.hpp"

// an entity is one or more sprites, cluttered together in an NxN tiles format
// all the information is stored row major. aka, the tileTableIndices for example, 
// stores the tiles (0, 0), (0, 1), (1, 0), (1, 1) respectively in a 2x2 tiles format.

// Upon drawing, the Scene will draw a collection of Entities, and all the indices will be
// offset by its global entity offset.

// In addition, entities will store their animation
class Entity
{
public:
    Entity() = default;

    void Update();

    // loads an image, and split it into a bunch of 8x8 sprites
    void Load(const std::string& relativePath);

    struct SpriteInfo
    {
        PPU::Tile tile;
        uint8_t palette; // index into local palette table
    };

    glm::vec2 position;

private:
    friend class Scene;

    void LoadSprites();

    void LoadOne(glm::u8vec4* pixels, uint32_t index);

    bool draw; // is this entity draw or no
    
    // info for all the sprites, ordered row major
    std::vector<SpriteInfo> info;

    // all the palettes used. Can be reduced in size later
    std::vector<PPU::Palette> palettes;

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