#pragma once

#include <glm/glm.hpp>
#include <vector>

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

    // loads an image, and split it into a bunch of 8x8 sprites
    void Load(const std::string& relativePath);

    struct SpriteInfo
    {
        PPU::Tile tile;
        uint8_t palette; // index into local palette table
    };

private:
    friend class Scene;

    void LoadSprites();

    void LoadOne(glm::u8vec4* pixels, uint32_t index);

    [[maybe_unused]] bool draw; // is this entity draw or no
    [[maybe_unused]] glm::uvec2 position;
    
    // info for all the sprites, ordered row major
    std::vector<SpriteInfo> info;

    // all the palettes used. Can be reduced in size later
    std::vector<PPU::Palette> palettes;
};