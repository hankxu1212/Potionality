#include "Scene.hpp"
#include "../core/Files.hpp"
#include "../core/utils/Logger.hpp"

#include <iostream>

Scene::Scene()
{
    entities.resize(1);
    entities[0] = std::make_unique<Entity>();
    entities[0]->Load("resources/test.png");
}

Scene::~Scene()
{
	Unload();
}

void Scene::Unload()
{
}

void Scene::Update()
{
    PPU::Get()->sprites.clear();
    PPU::Get()->palette_table.clear();
    
    uint32_t tileOffset = 0;

    for (auto& ent : entities)
    {
        // update palettes
        for (int p=0; p<ent->palettes.size(); ++p){
            PPU::Get()->palette_table.push_back(ent->palettes[p]);
        }

        for (int i=0; i<ent->info.size(); i++)
        {
            // update tiles
            PPU::Get()->tile_table[tileOffset] = ent->info[i].tile;

            // update sprites
            uint8_t attributes = ent->draw ? (uint8_t)i + (0x80) : (uint8_t)i;
            PPU::Get()->sprites.push_back(PPU::Sprite{
                (uint8_t)ent->position.x, 
                (uint8_t)ent->position.y,
                (uint8_t)tileOffset,
                attributes
            });

            tileOffset++;
        }
    }
}

void Scene::Render()
{

}