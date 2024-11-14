#pragma once

#include "PotionCore.hpp"
#include "../core/Module.hpp"

class SpriteRendererList;
class SpriteRenderer;

class RecipeManager : public InteractableObject
{
public:
	RecipeManager();

	void Awake() override;
	void Update() override;
	void Shutdown() override;
	void Interact(InteractPayload* payload = nullptr) override;

	void LoadRecipes(const std::string& path);

	bool HandleEvent(const SDL_Event& e) override;

	void OnLeftClicked();
	void OnRightClicked();

	SDL_Keycode leftKey = SDLK_z;
	SDL_Keycode rightKey = SDLK_x;

	SpriteRendererList* recipeRendererList;
	Entity* recipeRendererEntity;
	SpriteRenderer* mainBookSpriteRenderer;

	const float scaleAnimationMaxTime = 0.5f;
	float scaleAnimationTimer = 0;

	void HandleAnimations();
	glm::vec2 originalScale;
	float scaleChange = 2;
	glm::vec2 upScaled;

private:
	std::unordered_map<std::string, Recipe> m_Recipes;
};

