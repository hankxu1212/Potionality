#pragma once

/**
 * A script attached to in game entities (Monobehavior basically)
 * Should use the Create() function to create scripts so they get ported into script manager!
 */
class Behaviour
{
public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {};
	virtual void Shutdown() {}
	bool HandleEvent(const SDL_Event& e) { return false; }

	virtual const char* getClassName() const { return "empty"; }

	bool enabled;
};