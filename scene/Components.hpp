#pragma once

// add all your gameplay scripts here!
#include "../scripting/Player.hpp"
#include "../scripting/Customer.hpp"
#include "../scripting/Ingredient.hpp"
<<<<<<< HEAD
#include "../scripting/CuttingStation.hpp"
=======
#include "../scripting/Potion.hpp"
>>>>>>> 42d51b070f2659a210fd0f0ad7dc894c1ac668d5

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
