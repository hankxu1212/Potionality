#pragma once

// add all your gameplay scripts here!
#include "../scripting/Player.hpp"
#include "../scripting/Customer.hpp"
#include "../scripting/Ingredient.hpp"
#include "../scripting/WorkStation.hpp"
#include "../scripting/Potion.hpp"
#include "../scripting/Hearts.hpp"
#include "../scripting/DialogueUI.h"
#include "../scripting/RecipeManager.h"

// sprite components
#include "../sprites/SpriteLoader.h"
#include "../sprites/SpriteRenderer.h"
#include "../sprites/SpritesheetRenderer.h"
#include "../sprites/SpritesheetLoader.h"
#include "../sprites/SpriteRendererList.hpp"
#include "../physics/BoxCollider.hpp"

template<typename... C> struct ComponentGroup { };

using AllComponents = ComponentGroup
<
	Behaviour,
	SpriteLoader,
	SpriteRenderer,
	SpritesheetRenderer,
	SpritesheetLoader,
	BoxCollider
>;
