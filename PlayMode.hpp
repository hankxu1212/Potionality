#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <map>

struct PlayMode : Mode {
	virtual ~PlayMode();

	//functions called by main loop:
	virtual void Init() override;
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//some weird background animation:
	float background_fade = 0.0f;

	//player position:
	glm::vec2 player_at = glm::vec2(0.0f);

	//----- scene management -----
	// Scene scene;

	//----- program modules -----
	std::map<TypeId, std::unique_ptr<Module>>				m_Modules; // a list of program modules
	std::map<Module::UpdateStage, std::vector<TypeId>>		m_ModuleStages; // when to update them
	std::map<Module::DestroyStage, std::vector<TypeId>>		m_ModuleDestroyStages; // when to destroy them

	void UpdateModules();
	void DestroyModules();
	void CreateModule(Module::RegistryMap::const_iterator it);
	void DestroyModule(TypeId id, Module::DestroyStage stage);
	void UpdateStage(Module::UpdateStage stage);
	void DestroyStage(Module::DestroyStage stage);
};
