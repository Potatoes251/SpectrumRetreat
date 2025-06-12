#pragma once
#include <irrKlang.h>
#include <string>
#include <unordered_map>

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    void    playSound(const std::string& filePath, bool loop = false);
    void    playMusic(const std::string& filePath, bool loop = true);
    void    stopMusic();
    void    setMusicVolume(float volume); // 0.0 to 1.0
    void    setSoundVolume(float volume); // 0.0 to 1.0

private:
    irrklang::ISoundEngine*     m_engine;
    irrklang::ISound*           m_currentMusic = nullptr;

    float   m_musicVolume = 1.0f;
    float   m_soundVolume = 1.0f;
};