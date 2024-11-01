#pragma once
#include <string>
#include <unordered_map>
#include <vector>

enum class Action
{
	None = -1,
	Smash = 0,
	Eat = 1,
	Brew = 2,
	Cut = 3
};

struct ActionSpecification
{
	std::string sourceIngredient;
	Action action;
	std::string destinationIngredient;
};

struct ActionState
{
	float progress = 0;
};

struct Potion
{
	std::string name;
};

struct Ingredient_T
{
	std::string name;
	std::vector<Action> executableActions;
	std::unordered_map<Action, ActionState> actionStates;
};

/*
struct Ingredient
{
	Ingredient_T* ingredient;

	void Update()
	{
		// check if actionProgress has fufilled 
	}
};
*/

struct Recipe
{
	std::vector<ActionSpecification> actionsToTake;
	Potion potion;
};

struct PotionRequest
{
	std::vector<Potion> potionsToMake;
	float reward;
};