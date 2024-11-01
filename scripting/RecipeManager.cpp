#include "RecipeManager.h"
#include "../core/Files.hpp"
#include "../core/utils/sejp/sejp.hpp"
#include "../core/utils/Logger.hpp"

RecipeManager::RecipeManager()
{
    LoadRecipes("../resources/scenes/Recipes.json");
}

RecipeManager::~RecipeManager()
{
}

static Action StringToAction(const std::string& actionString)
{
    if (actionString == "Smash") return Action::Smash;
    if (actionString == "Eat") return Action::Eat;
    if (actionString == "Brew") return Action::Brew;
    if (actionString == "Cut") return Action::Cut;
    return Action::None;
}

std::string ActionToString(Action action)
{
    switch (action)
    {
    case Action::Smash: return "Smash";
    case Action::Eat: return "Eat";
    case Action::Brew: return "Brew";
    case Action::Cut: return "Cut";
    default: return "Unknown";
    }
}

void RecipeManager::LoadRecipes(const std::string& path)
{
    try {
        sejp::value loaded = sejp::load(Files::Path(path));
        auto& map = loaded.as_object().value();
        LOG_DEBUG("Loaded recipe from " + path, Logger::CYAN, Logger::BOLD);

        const auto& recipeArr = map.at("recipes").as_array().value();
        for (int i = 0; i < recipeArr.size(); i++)
        {
            Recipe newRecipe;
            newRecipe.actionsToTake.clear();

            const auto& val = recipeArr[i].as_object().value();
            newRecipe.potion.name = val.at("potion").as_string().value();

            const auto& actions = val.at("actions").as_array().value();

            for (int j = 0; j < actions.size(); ++j)
            {
                const auto& step = actions[j].as_object().value();

                ActionSpecification actionToTake;

                actionToTake.sourceIngredient = step.at("src").as_string().value();
                actionToTake.destinationIngredient = step.at("dst").as_string().value();
                actionToTake.action = StringToAction(step.at("action").as_string().value());

                newRecipe.actionsToTake.emplace_back(actionToTake);
            }
                
            const std::string& name = val.at("name").as_string().value();
            if (m_Recipes.find(name) != m_Recipes.end())
                throw std::runtime_error("Recipe with name " + name + " already exists!");

            m_Recipes[name] = newRecipe;
        }

        for (auto& [key, val] : m_Recipes)
        {
            std::cout<< "Recipe: " << val.potion.name << " requires actions: ";
            for (auto& action : val.actionsToTake)
            {
                std::cout << "[ action: " << ActionToString(action.action) << ", src: " << action.sourceIngredient << ", dst: " << action.destinationIngredient << "]";
            }
            std::cout << std::endl;
        }
    }
    catch (std::exception& e) {
        LOG_ERROR_F("Failed to deserialize a scene with path: [{}]. Error: {}", std::move(path), e.what());
    }
}
