#include "Player.hpp"
#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"

SpritesheetRenderer* playerSprite;

void Player::Awake()
{
	if (Player::Instance)
	{
		LOG_WARN("Multiple player instances found!");
	}

	Player::Instance = this;

	playerSprite = entity->GetComponent<SpritesheetRenderer>();
}

void Player::Start()
{
}

void Player::Update()
{
	assert(entity);

	HandleMovement();
	HandleState();

	// reset buttons
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
		}else if (evt.key.keysym.sym == interactKey) {
			interact.downs += 1;
			interact.pressed = true;
			OnInteractPressed();
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
		} else if (evt.key.keysym.sym == interactKey) {
			interact.pressed = false;
			return true;
		}
	}

	return false;
}

void Player::OnInteractPressed()
{
	LOG_INFO("Interact!");
}

void Player::HandleMovement()
{
	constexpr float PlayerSpeed = 200.0f;

	glm::vec2 direction = glm::vec2(0);

	if (left.pressed) direction += glm::vec2(-1, 0);
	if (right.pressed) direction += glm::vec2(1, 0);
	if (down.pressed) direction += glm::vec2(0, 1);
	if (up.pressed) direction += glm::vec2(0, -1);

	if (interact.pressed)
	{
		// Don't move if currently smashing
		// TODO: Might want to change this later
		m_PlayerState = State::Smash;
	}
	else if (direction != glm::vec2(0))
	{
		m_PlayerState = State::Walk;
		GetTransform()->position += PlayerSpeed * Time::DeltaTime * normalize(direction);
	}
	else
	{
		m_PlayerState = State::Idle;
	}

	// Add more controls here for smash!
}

void Player::HandleState()
{
	if (m_PlayerState == State::Idle)
	{
		playerSprite->SetLoopRegion(1, 3);
	}
	else if (m_PlayerState == State::Walk)
	{
		playerSprite->SetLoopRegion(2, 3);
	}
	else if (m_PlayerState == State::Smash)
	{
		playerSprite->SetLoopRegion(3, 5);
	}
}

int Player::GetSmashes() {
	int smashes = interact.downs;
	interact.downs = 0;
	return smashes;
}

void AddIngredient(uint64_t ingredient_id) {
	ingredients.emplace_back(ingredient_id);
}

std::list<uint64_t> GetIngredients() {
	return ingredients;
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