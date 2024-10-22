#include "Scene.hpp"
#include "../core/Files.hpp"
#include "../core/utils/Logger.hpp"

#include <iostream>
#include "Entity.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
	Unload();
}

void Scene::Load()
{
    auto e = Instantiate();
    // e->Load("../resources/test.png", 2, 2);
    e->Load("../resources/test.png", 1, 4);
    e->AddComponent<Player>();
}

void Scene::Unload()
{
}

void Scene::Update()
{
    for (auto& ent : entities)
    {
        ent->Update();
    }
}

void Scene::Render()
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
                (uint8_t)(ent->position.x + ent->spriteOffsets[i].x),
                (uint8_t)(ent->position.y + ent->spriteOffsets[i].y),
                (uint8_t)tileOffset,
                attributes
            });

            tileOffset++;
        }
    }
}