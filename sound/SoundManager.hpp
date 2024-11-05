#pragma once

#include "../core/Module.hpp"
#include "Sound.hpp"
#include "../scene/RectTransform.hpp"

class SoundManager : public Module::Registrar<SoundManager>
{
	inline static const bool Registered = Register(UpdateStage::Never, DestroyStage::Normal);

public:
	SoundManager();

	virtual ~SoundManager();

    using EventInstance = std::shared_ptr<Sound::PlayingSample>;

	void Update();

    void SetConstantListener(const glm::vec2& position);

    void SetFollowListener(RectTransform* transform);

    void LoadSample(const std::string& path, const std::string& handle);

    void PlayOneShot(const std::string& handle, float volume=5);

    void PlayOneShot(const std::string& handle, const RectTransform& transform, float volume=5);

    EventInstance InstantiateSoundInstance(const std::string& handle, const RectTransform& transform);

    void StopInstance(EventInstance instance);

private:
    // a map of active instances
    std::unordered_map<EventInstance, RectTransform*> m_EventInstances;

    // handle to sound sample 
    std::unordered_map<std::string, std::unique_ptr<Sound::Sample>> m_SoundSamples;

    RectTransform* m_SoundListener;
};