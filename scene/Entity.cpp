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

void Entity::Load(const std::string& path)
{
    // load a 4 byte per pixel RGBA image
    Bitmap bitmap(path);
    LOG_INFO_F("Found bitmap length :{}", bitmap.GetLength());

	// Reinterpret the data as an array of glm::u8vec4
	[[maybe_unused]] glm::u8vec4* pixels = reinterpret_cast<glm::u8vec4*>(bitmap.data.get());

    size_t numPixels = bitmap.GetLength() / sizeof(glm::u8vec4);  // Make sure to calculate the correct size
    assert(numPixels == 8 * 8); // for now, only support loading a single sprite

    // [[maybe_unused]] std::unordered_map<glm::u8vec4> colors;

    // for (size_t i = 0; i < pixels; ++i) 
    // {
	// 	glm::u8vec4 color = pixels[i];

    //     auto it = colors.find(color)
    //     if (colors.find(color))
    //     if ()
    // }
}