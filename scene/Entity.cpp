#include "Entity.hpp"
#include "../core/utils/Bitmap.hpp"
#include "../core/utils/Logger.hpp"
#include "../PPU466.hpp"
#include <unordered_map>

#include <glm/glm.hpp>
#include <functional> // for std::hash

namespace std {
    template <>
    struct hash<glm::u8vec4> {
        std::size_t operator()(const glm::u8vec4& vec) const {
            std::size_t h1 = std::hash<uint8_t>{}(vec.r);
            std::size_t h2 = std::hash<uint8_t>{}(vec.g);
            std::size_t h3 = std::hash<uint8_t>{}(vec.b);
            std::size_t h4 = std::hash<uint8_t>{}(vec.a);

            // Combine the hashes with bit shifts and XOR
            return h1 ^ (h2 << 8) ^ (h3 << 16) ^ (h4 << 24);
        }
    };
}

uint32_t ColorDistance(const glm::u8vec4& a, const glm::u8vec4& b) {
    return (a.r - b.r) * (a.r - b.r) +
        (a.g - b.g) * (a.g - b.g) +
        (a.b - b.b) * (a.b - b.b);
}

static uint8_t FindClosestIndex(glm::u8vec4 col, const std::unordered_map<glm::u8vec4, uint8_t>& colors)
{
    std::vector<std::pair<uint8_t, uint32_t>> indexToDistance;
    for (const auto& [c, index] : colors)
    {
        uint32_t d = ColorDistance(col, c);
        indexToDistance.emplace_back(std::make_pair(index, d));
    }

    std::sort(indexToDistance.begin(), indexToDistance.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    return indexToDistance[0].first;
}

void Entity::Load(const std::string& path)
{
    // load a 4 byte per pixel RGBA image
    Bitmap bitmap(path);
    LOG_INFO_F("Found bitmap length :{}", bitmap.GetLength());

	// Reinterpret the data as an array of glm::u8vec4
	glm::u8vec4* pixels = reinterpret_cast<glm::u8vec4*>(bitmap.data.get());

    size_t numPixels = bitmap.GetLength() / sizeof(glm::u8vec4);  // Make sure to calculate the correct size
    assert(numPixels == 8 * 8); // for now, only support loading a single sprite

    PPU::Tile tile;
    palettes.resize(1); // temporary

    std::unordered_map<glm::u8vec4, uint8_t> colorToPaletteIndex;
    std::unordered_map<glm::u8vec4, uint32_t> colorFrequencies;
    // first pass: find the colors with the highest frequency
    for (size_t i = 0; i < numPixels; ++i) 
    {
		glm::u8vec4 color = pixels[i];

        auto it = colorToPaletteIndex.find(color);
        if (it != colorToPaletteIndex.end()){
            tile.pixels[i] = it->second;
        }
        else{
            int nextIndex = currentColor + 1;

            if (nextIndex >= PPU::PALETTE_SIZE){
                tile.pixels[i] = FindClosestIndex(color, colorToPaletteIndex);
                continue;
            }
            
            palettes[0][nextIndex] = color;
            tile.pixels[i] = nextIndex;
            
            colorToPaletteIndex[color] = nextIndex;
            
            currentColor++;
        }
    }

    SpriteInfo newInfo;
    newInfo.tile = tile;
    newInfo.palette = 0;

    std::vector<glm::u8vec4> bytes;
    for (size_t i = 0; i < numPixels; ++i) 
    {
        bytes.emplace_back(palettes[0][tile.pixels[i]]);
    }

    Bitmap bit(glm::vec2{8,8});
    memcpy(bit.data.get(), bytes.data(), bit.GetLength());
    bit.Write("test.png");

    info.emplace_back(newInfo);
}