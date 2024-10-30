#pragma once

#include "PotionCore.hpp"
#include "../core/Module.hpp"

class RecipeManager : public Module::Registrar<RecipeManager>
{
	inline static const bool Registered = Register(UpdateStage::Never, DestroyStage::Normal);
public:
	RecipeManager();

	virtual ~RecipeManager();

	void LoadRecipes(const std::string& path);

private:
	std::unordered_map<std::string, Recipe> m_Recipes;
};

