#include "Entity.hpp"
#include "../core/utils/Bitmap.hpp"
#include "../core/utils/Logger.hpp"

void Entity::Load(const std::string& path)
{
    Bitmap bitmap(path);
    LOG_INFO_F("Found bitmap len:{}", bitmap.GetLength());
}