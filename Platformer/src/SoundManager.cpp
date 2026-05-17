#include "SoundManager.h"

SoundManager::SoundManager()
{
    m_engine = irrklang::createIrrKlangDevice();
}

SoundManager::~SoundManager()
{
    if (m_engine)
    {
        m_engine -> drop();
        m_engine = nullptr;
    }
}

void    SoundManager::playSound(const std::string& filePath, bool loop)
{
    if (m_engine)
    {
        m_engine -> play2D(filePath.c_str(), loop, false, true) -> setVolume(m_soundVolume);
    }
}

void    SoundManager::playMusic(const std::string& filePath, bool loop)
{
    stopMusic(); // stop previous music
    if (m_engine)
    {
        m_currentMusic = m_engine -> play2D(filePath.c_str(), loop, false, true);

        if (m_currentMusic)
            m_currentMusic -> setVolume(m_musicVolume);
    }
}

void    SoundManager::stopMusic()
{
    if (m_currentMusic)
    {
        m_currentMusic -> stop();
        m_currentMusic -> drop();
        m_currentMusic = nullptr;
    }
}

void    SoundManager::setMusicVolume(float volume)
{
    m_musicVolume = volume;

    if (m_currentMusic)
        m_currentMusic -> setVolume(volume);
}

void    SoundManager::setSoundVolume(float volume)
{
    m_soundVolume = volume;
}