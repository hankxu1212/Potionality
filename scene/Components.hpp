#pragma once

// add all your gameplay scripts here!
#include "../scripting/Player.hpp"
#include "../scripting/Customer.hpp"
#include "../scripting/Ingredient.hpp"
#include "../scripting/CuttingStation.hpp"
#include "../scripting/Potion.hpp"
#include "../scripting/DialogueUI.h"

// sprite components
#include "../sprites/SpriteLoader.h"
#include "../sprites/SpriteRenderer.h"
#include "../sprites/SpritesheetRenderer.h"
#include "../sprites/SpritesheetLoader.h"

template<typename... C> struct ComponentGroup { };

using AllComponents = ComponentGroup
<
	Behaviour,
	SpriteLoader,
	SpriteRenderer,
	SpritesheetRenderer,
	SpritesheetLoader
>;
