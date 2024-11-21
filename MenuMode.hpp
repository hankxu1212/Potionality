#pragma once

#include "Mode.hpp"

#include <glm/glm.hpp>

struct MenuMode : Mode
{
	virtual ~MenuMode();
	//functions called by main loop:
	virtual void Init() override;
	virtual bool handle_event(SDL_Event const&, glm::uvec2 const& window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const& drawable_size) override;

	void OnUpPressed();
	void OnDownPressed();
	void OnSpacePressed();

	int m_CurrentOption = 0;
	
	float positions[2] = { 750,  860 };
};

