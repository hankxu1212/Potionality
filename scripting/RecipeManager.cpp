#include "RecipeManager.h"

#include "../core/Files.hpp"
#include "../core/utils/sejp/sejp.hpp"
#include "../core/utils/Logger.hpp"

#include "../scene/Scene.hpp"
#include "../scene/Entity.hpp"

#include "../sprites/SpriteRendererList.hpp"
#include "../PlayMode.hpp"

#include "../sound/SoundManager.hpp"

RecipeManager::RecipeManager()
{
    LoadRecipes("resources/scenes/Recipes.json");
}

void RecipeManager::Awake()
{
    InteractableObject::Awake();

    recipeRendererEntity = entity->AddChild("recipe list", {0,-512}, {300, 300}, 0, 5);
    recipeRendererEntity->AddComponent<SpriteLoader>(SPRITE_SHADER);
    recipeRendererList = &recipeRendererEntity->AddComponent<SpriteRendererList>(true);
    recipeRendererList->AppendSprite("LovePotionRecipe");
    recipeRendererList->AppendSprite("RedPotionRecipe");
    recipeRendererList->AppendSprite("BluePotionRecipe");
    recipeRendererList->AppendSprite("GreenPotionRecipe");
    recipeRendererList->AppendSprite("PoisonPotionRecipe");

    // will adjust, so it looks better
    blankRecipeRendererEntity = entity->AddChild("recipe blankpg", {-400,-512}, {300, 300}, 0, 4);
    blankRecipeRendererEntity->AddComponent<SpriteLoader>(SPRITE_SHADER);
    blankRecipeRendererList = &blankRecipeRendererEntity->AddComponent<SpriteRendererList>(true);
    blankRecipeRendererList->AppendSprite("BlankPageRecipe");
    


    mainBookSpriteRenderer = entity->GetComponent<SpriteRenderer>();
    
    assert(mainBookSpriteRenderer && recipeRendererList && "Need a sprite renderer list and a main book sprite renderer!");

    originalScale = recipeRendererEntity->transform()->size();
    upScaled = originalScale * scaleChange;
}

void RecipeManager::Update()
{
    InteractableObject::Update(); // need to call baseclass explicitly

    if (!isCurrentInteractable) 
    {
        recipeRendererList->currentSpriteIndex = -1;
        blankRecipeRendererList->currentSpriteIndex = -1;
        mainBookSpriteRenderer->isActive = true;
    }

    HandleAnimations();
}

void RecipeManager::Shutdown()
{
    InteractableObject::Shutdown();
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
            //newRecipe.potion.name = val.at("potion").as_string().value();
            newRecipe.potionName = val.at("potion").as_string().value();

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
            std::cout<< "Recipe: " << val.potionName << " requires actions: "; // Note to self: val.potion.name if recipe ends up having a Potion
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

bool RecipeManager::HandleEvent(const SDL_Event& evt)
{
    if (evt.type == SDL_KEYDOWN) {
        if (evt.key.keysym.sym == leftKey) {
            OnLeftClicked();
            return true;
        }else if (evt.key.keysym.sym == rightKey) {
            OnRightClicked();
            return true;
        }
    }
    return false;
}

void RecipeManager::OnLeftClicked()
{
    if (recipeRendererList->currentSpriteIndex == -1)
        return;

    blankRecipeRendererList->currentSpriteIndex=0;
    recipeRendererList->currentSpriteIndex--;
    if (recipeRendererList->currentSpriteIndex < 0)
        recipeRendererList->currentSpriteIndex += (int)recipeRendererList->sprites.size();

    recipeRendererList->currentSpriteIndex %= recipeRendererList->sprites.size();

    SoundManager::Get()->PlayOneShot("BookTurn2SFX", 1);
}

void RecipeManager::OnRightClicked()
{
    if (recipeRendererList->currentSpriteIndex == -1)
        return;

    blankRecipeRendererList->currentSpriteIndex=0;
    recipeRendererList->currentSpriteIndex++;
    recipeRendererList->currentSpriteIndex %= recipeRendererList->sprites.size();

    SoundManager::Get()->PlayOneShot("BookTurnSFX", 1);
}

void RecipeManager::HandleAnimations()
{
    if (scaleAnimationTimer <= 0)
        return;

    scaleAnimationTimer -= Time::DeltaTime;
    float t = 1 - scaleAnimationTimer / scaleAnimationMaxTime;

    recipeRendererEntity->transform()->SetSize(glm::mix(originalScale, upScaled, t));
    blankRecipeRendererEntity->transform()->SetSize(glm::mix(originalScale, upScaled, t));
}

void RecipeManager::Interact(InteractPayload* payload)
{
    SoundManager::Get()->PlayOneShot("BookTurnSFX", 1);

    recipeRendererList->currentSpriteIndex = 0;
    blankRecipeRendererList->currentSpriteIndex=0;
    mainBookSpriteRenderer->isActive = false;

    scaleAnimationTimer = scaleAnimationMaxTime;
}

SETUP_DEFAULT_CALLBACKS(RecipeManager)