#pragma once

#include "../core/Module.hpp"
#include "Scene.hpp"

/**
 * Holds and updates the unique pointer to the active scene.
 */
class SceneManager : public Module::Registrar<SceneManager>
{
	inline static const bool Registered = Register(UpdateStage::Pre, DestroyStage::Normal);

public:
	SceneManager();

	~SceneManager();

	void LoadActiveScene();

	/**
	 * Updates the active scene.
	 */
	void Update();

	void Shutdown();

	/**
	 * Gets the current scene.
	 * \return The current scene.
	 */
	inline Scene* getScene() const { return scene.get(); }
private:
	std::unique_ptr<Scene> scene; // the active scene
};
