#include "Entity.hpp"
#include "../core/utils/Bitmap.hpp"
#include "../core/utils/Logger.hpp"
#include "../PPU466.hpp"
#include "../core/Files.hpp"

#include <unordered_map>
#include <unordered_set>
#include <glm/glm.hpp>
#include <functional> // for std::hash

// #define _DEBUG

constexpr uint32_t NUM_PIXELS_PER_SPRITE = 64;

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

static uint32_t ColorDistance(const glm::u8vec4& a, const glm::u8vec4& b) {
    return (a.r - b.r) * (a.r - b.r) +
        (a.g - b.g) * (a.g - b.g) +
        (a.b - b.b) * (a.b - b.b);
}

static uint8_t FindClosestIndex(glm::u8vec4 col, const PPU::Palette& palette)
{
    uint32_t min_d = INT32_MAX;
    int min_id = -1;

    for (int i=0;i<palette.size();i++)
    {
        uint32_t d = ColorDistance(col, palette[i]);
        if (d < min_d)
        {
            min_d = d;
            min_id = i;
        }
    }
    return uint8_t(min_id);
}

void* OffsetPointer(void* ptr, size_t offset) {
	return static_cast<void*>(static_cast<std::byte*>(ptr) + offset);
}

void Entity::Update()
{
    // update components
	for (auto& component : m_Components)
	{
		component->Update();
	}
}

// Function to transpose a vector of data from column-major to row-major order
template<typename T>
std::vector<T> TransposeColumnToRowMajor(const std::vector<T>& input, int gridWidth, int gridHeight) {
    std::vector<T> output(gridWidth * gridHeight);

    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            output[row * gridWidth + col] = input[col * gridHeight + row];
        }
    }

    return output;
}

static std::vector<glm::vec2> CalculateSpriteOffsets(int gridWidth, int gridHeight) {
    constexpr int spriteSize = 8;

    std::vector<glm::vec2> offsets;

    // Loop through each sprite in the grid
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            // Origin of the current sprite (top-left corner)
            float spriteOriginX = x * spriteSize;
            float spriteOriginY = y * spriteSize;

            // Calculate offset from the sprite's origin to the grid center
            glm::vec2 offset(spriteOriginX, spriteOriginY);

            offsets.push_back(offset);
        }
    }

    return offsets;

    // return TransposeColumnToRowMajor(offsets, gridWidth, gridHeight);
}

// Function to convert a flat pixel array into 2D blocks of 8x8 pixels
static std::vector<std::vector<glm::u8vec4>> ConvertTo8x8Blocks(const glm::u8vec4* pixels, int numBlocksX, int numBlocksY) {
    std::vector<std::vector<glm::u8vec4>> blocks(numBlocksX * numBlocksY, std::vector<glm::u8vec4>(NUM_PIXELS_PER_SPRITE));  // 64 pixels per block

    uint32_t imageWidth = numBlocksX * 8;

    // Iterate over the entire image, converting the pixels into 8x8 blocks
    for (int blockY = 0; blockY < numBlocksY; ++blockY) {
        for (int blockX = 0; blockX < numBlocksX; ++blockX) {
            // Current block index in the 1D blocks vector
            int blockIndex = blockY * numBlocksX + blockX;

            // Fill the current 8x8 block with pixels from the original pixel array
            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    // Calculate the corresponding pixel index in the original pixel array
                    int pixelIndex = (blockY * 8 + y) * imageWidth + (blockX * 8 + x);

                    // Assign the pixel to the correct position in the 8x8 block
                    blocks[blockIndex][y * 8 + x] = pixels[pixelIndex];
                }
            }
        }
    }

    return blocks;
}

void Entity::Load(const std::string &path, uint32_t width, uint32_t height)
{
    // load a 4 byte per pixel RGBA image
    Bitmap bitmap(Files::Path(path));
    LOG_INFO_F("Found bitmap length :{}", bitmap.GetLength());

    uint32_t numTiles = bitmap.size.x * bitmap.size.y / NUM_PIXELS_PER_SPRITE;
    assert(numTiles == width * height);

	// Reinterpret the data as an array of glm::u8vec4
	glm::u8vec4* pixels = reinterpret_cast<glm::u8vec4*>(bitmap.data.get());

    // arrange into a 2D array of 8x8 pixel blocks:
    std::vector<std::vector<glm::u8vec4>> tiled_pixels = ConvertTo8x8Blocks(pixels, width, height);

    for (uint32_t i=0; i<numTiles; ++i){
        LoadOne(tiled_pixels[i].data(), (size_t)i);
    }
    LOG_INFO_F("Loaded {} into {} tiles and {} palettes", path, numTiles, palettes.size());

    auto offsets = CalculateSpriteOffsets(width, height);
    spriteOffsets.assign(offsets.begin(), offsets.end());
}

void Entity::LoadOne(glm::u8vec4 *pixels, uint32_t index)
{
    PPU::Tile tile;
    PPU::Palette palette;

    // first pass: find the colors with the highest frequency
    std::unordered_map<glm::u8vec4, uint32_t> colorFrequencies;
    for (size_t i = 0; i < NUM_PIXELS_PER_SPRITE; ++i) 
    {
        glm::u8vec4 color = pixels[i];
        auto it = colorFrequencies.find(color);
        if (it != colorFrequencies.end())
            it->second++;
        else
            colorFrequencies[color] = 1;
    }

    // sort colors by frequency
    std::vector<std::pair<glm::u8vec4, uint32_t>> topColors;
    for (const auto& [c, frequency] : colorFrequencies){
        topColors.emplace_back(std::make_pair(c, frequency));
    }
    std::sort(topColors.begin(), topColors.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // choose PALETTE_SIZE=16 colors to use
    std::unordered_map<glm::u8vec4, uint8_t> colorsToUse;
    for (int i=0;i<fmin(PPU::PALETTE_SIZE, topColors.size());++i){
        colorsToUse[topColors[i].first] = (uint8_t)i;
        palette[i] = topColors[i].first;
    }

    // second pass: all the colors, pick existing top color or 
    // choose the closest one to some top color
    for (size_t i = 0; i < NUM_PIXELS_PER_SPRITE; ++i) 
    {
		glm::u8vec4 color = pixels[i];

        auto it = colorsToUse.find(color);
        if (it != colorsToUse.end()){
            tile.pixels[i] = it->second;
        }
        else{
            tile.pixels[i] = FindClosestIndex(color, palette);
        }
    }

    palettes.emplace_back(palette);
    info.emplace_back(SpriteInfo{tile, (uint8_t)index});

#ifdef _DEBUG
    // write, for debugging
    std::vector<glm::u8vec4> bytes;
    for (size_t i = 0; i < NUM_PIXELS_PER_SPRITE; ++i) 
    {
        bytes.emplace_back(palette[tile.pixels[i]]);
    }

    Bitmap bit(glm::vec2{8,8});
    memcpy(bit.data.get(), bytes.data(), bit.GetLength());
    bit.Write("test" + std::to_string(index) + ".png");
   #endif
}
