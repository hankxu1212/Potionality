#include "Player.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"

void Player::Awake()
{
	if (Player::Instance)
	{
		LOG_WARN("Multiple player instances found!");
	}

	Player::Instance = this;
}

void Player::Start()
{
}

void Player::Update()
{
    if (!entity)
        LOG_WARN("NOT ENRTITY");

    constexpr float PlayerSpeed = 100.0f;
	if (left.pressed) GetTransform()->position.x -= PlayerSpeed * Time::DeltaTime;
	if (right.pressed) GetTransform()->position.x += PlayerSpeed * Time::DeltaTime;
	if (down.pressed) GetTransform()->position.y += PlayerSpeed * Time::DeltaTime;
	if (up.pressed) GetTransform()->position.y -= PlayerSpeed * Time::DeltaTime;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void Player::Shutdown()
{
}

bool Player::HandleEvent(const SDL_Event &evt)
{
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}
	}

	return false;
}

template<>
void Scene::OnComponentAdded<Player>(Entity& entity, Player& component)
{
	this->OnComponentAdded<Behaviour>(entity, component);
}

template<>
void Scene::OnComponentRemoved<Player>(Entity& entity, Player& component)
{
	this->OnComponentRemoved<Behaviour>(entity, component);
}