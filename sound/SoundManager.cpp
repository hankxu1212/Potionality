#include "SoundManager.hpp"

#include "../core/Files.hpp"
#include "../core/utils/Logger.hpp"

SoundManager::SoundManager()
{
    Sound::init();

    LoadSample("resources/sound/CartoonBite.wav", "BiteSFX");
    LoadSample("resources/sound/Congratulations.wav", "SuccessSFX");
    LoadSample("resources/sound/Negative.wav", "FailSFX");
    LoadSample("resources/sound/Type.wav", "TypeSFX");
    LoadSample("resources/sound/InteractDisappear.wav", "InteractDisappearSFX");
    LoadSample("resources/sound/InteractAppear.wav", "InteractAppearSFX");
    LoadSample("resources/sound/BookTurn.wav", "BookTurnSFX");
    LoadSample("resources/sound/BookTurn2.wav", "BookTurn2SFX");
    LoadSample("resources/sound/OST.wav", "OST");
    LoadSample("resources/sound/LightSwitch.wav", "LightSwitchSFX");

    SoundManager::Loop("OST", 5);
}

SoundManager::~SoundManager()
{
    Sound::shutdown();
}

void SoundManager::Update()
{
    // update sound listener
    if (m_SoundListener)
    {
        glm::vec3 listenPosition(m_SoundListener->position().x, m_SoundListener->position().y, 0);
        Sound::listener.set_position_right(listenPosition, glm::vec3(1.0f, 0.0f, 0.0f));
    }
}

void SoundManager::SetConstantListener(const glm::vec2 &position)
{
    glm::vec3 listenPosition(position.x, position.y, 0);
    Sound::listener.set_position_right(listenPosition, glm::vec3(1.0f, 0.0f, 0.0f));
}

void SoundManager::SetFollowListener(RectTransform *transform)
{
    m_SoundListener = transform;
}

void SoundManager::LoadSample(const std::string &path, const std::string &handle)
{
    if (auto it = m_SoundSamples.find(handle); it != m_SoundSamples.end())
        return;
    m_SoundSamples[handle] = std::make_unique<Sound::Sample>(Files::Path(path));

    LOG_INFO_F("Loaded sound file at: {}", path);
}

void SoundManager::PlayOneShot(const std::string &handle, float volume)
{
    if (!m_SoundListener){
        LOG_WARN("Could not find sound listener. Use PlayOneShot(handle, transform) instead!");
        return;
    }

    auto it = m_SoundSamples.find(handle);
    if (it == m_SoundSamples.end()){
        LOG_INFO_F("Could not find sound file handle: {}", handle);
        return;
    }

    glm::vec3 listenPosition(m_SoundListener->position().x, m_SoundListener->position().y, 0);
    Sound::play_3D(*it->second.get(), volume, listenPosition);
}

void SoundManager::PlayOneShot(const std::string &handle, const RectTransform &transform, float volume)
{
    auto it = m_SoundSamples.find(handle);
    if (it == m_SoundSamples.end()){
        LOG_INFO_F("Could not find sound file handle: {}", handle);
        return;
    }

    glm::vec3 listenPosition(transform.position().x, transform.position().y, 0);
    Sound::play_3D(*it->second.get(), volume, listenPosition);
}

void SoundManager::Loop(const std::string& handle, float volume)
{
    auto it = m_SoundSamples.find(handle);
    if (it == m_SoundSamples.end()) {
        LOG_INFO_F("Could not find sound file handle: {}", handle);
        return;
    }

    Sound::loop(*it->second.get(), volume);
}

SoundManager::EventInstance SoundManager::InstantiateSoundInstance(const std::string &handle, const RectTransform &transform)
{
    LOG_WARN("Unimplemented function!");
    return EventInstance();
}

void SoundManager::StopInstance(EventInstance instance)
{
    LOG_WARN("Unimplemented function!");
}
