#pragma once

#include <SDL.h>
#include "../scene/Component.hpp"
#include "../core/Time.hpp"
#include "../core/utils/Logger.hpp"

/**
 * A script attached to in game entities (Monobehavior basically)
 * Should use the Create() function to create scripts so they get ported into script manager!
 */
class Behaviour : public Component
{
public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {};
	virtual void Shutdown() {}
	virtual bool HandleEvent(const SDL_Event& e) { return false; }

	virtual const char* getClassName() const { return "empty"; }
};