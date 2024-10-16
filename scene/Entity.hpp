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

private:
    [[maybe_unused]] glm::uvec2 position;
    [[maybe_unused]] std::vector<uint8_t> tileTableIndices; // index into global PPU table
    [[maybe_unused]] std::vector<uint8_t> paletteIndices; // index into local palette
    [[maybe_unused]] std::vector<uint8_t> priorityBits; // whether each of the tiles should be drawn
    [[maybe_unused]] std::vector<PPU::Palette> palettes;
};