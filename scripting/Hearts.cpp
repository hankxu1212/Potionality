#include "Hearts.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"
#include "Player.hpp"
#include "../PlayMode.hpp"
#include "../sprites/SpriteRenderer.h"

void Hearts::Awake()
{
    playerTransform = Player::Instance->GetTransform();
    idleTime = idleTimeMax;
    idleFlag = true;
}

void Hearts::Shutdown()
{
}

void Hearts::Update()
{
    if (idleTime > 0) {
        idleTime -= Time::DeltaTime;
        if (idleTime <= 0) {
            idleTime = idleTimeMax;
            idleFlag = !idleFlag;
        }
    }

    float x = playerTransform->position().x;
    float y = playerTransform->position().y;
    if (playerIsBig) {
        if (idleFlag) {
            GetTransform()->SetPosition(x + 110.0f, y + 120.0f);
        } else {
            GetTransform()->SetPosition(x + 110.0f, y + 110.0f);
        }
    } else {
        if (idleFlag) {
            GetTransform()->SetPosition(x + 10.0f, y + 20.0f);
        } else {
            GetTransform()->SetPosition(x + 10.0f, y + 10.0f);
        }
    }
}

SETUP_DEFAULT_CALLBACKS(Hearts)
