#pragma once

#include <string>
#include <SDL.h>

/**
  * A application layer. Can be attached and detached on runtime.
  * Destruction/Detachment order between layers are not reinforced and runs in the order 
  * they were initially pushed into the stack
*/
class Layer
{
public:
	Layer(const std::string& _name = "DefaultLayer") : name(_name) {}
	virtual ~Layer() = default; // correspond to between Normal and Post destruction stage

	virtual void OnAttach() {}
	virtual void OnDetach() {} // corresponds to between Pre and Normal destruction stage
	virtual void OnUpdate() {}
	virtual bool OnEvent(const SDL_Event& event) {return false;}
public:
	std::string name;
};
