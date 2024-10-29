#include "SoundManager.hpp"

#include "../core/Files.hpp"
#include "../core/utils/Logger.hpp"

SoundManager::SoundManager()
{
    Sound::init();

    LoadSample("../resources/sound/CartoonBite.wav", "BiteSFX");
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
        glm::vec3 listenPosition(m_SoundListener->position.x, m_SoundListener->position.y, 0);
        Sound::listener.set_position_right(listenPosition, glm::vec3(1.0f, 0.0f, 0.0f));
    }
}

void SoundManager::SetConstantListener(const glm::vec2 &position)
{
    glm::vec3 listenPosition(position.x, position.y, 0);
    Sound::listener.set_position_right(listenPosition, glm::vec3(1.0f, 0.0f, 0.0f));
}

void SoundManager::SetFollowListener(RectTransform &transform)
{
    m_SoundListener = &transform;
}

void SoundManager::LoadSample(const std::string &path, const std::string &handle)
{
    if (auto it = m_SoundSamples.find(handle); it != m_SoundSamples.end())
        return;
    m_SoundSamples[handle] = std::make_unique<Sound::Sample>(Files::Path(path));

    LOG_INFO_F("Loaded sound file at: {}", path);
}

void SoundManager::PlayOneShot(const std::string &handle)
{
}

void SoundManager::PlayOneShot(const std::string &handle, const RectTransform &transform)
{
}

SoundManager::EventInstance SoundManager::InstantiateSoundInstance(const std::string &handle, const RectTransform &transform)
{
    return EventInstance();
}

void SoundManager::StopInstance(EventInstance instance)
{
}
