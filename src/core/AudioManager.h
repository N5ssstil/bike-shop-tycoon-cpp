#pragma once

#include <string>
#include <unordered_map>
#include <SDL2/SDL_mixer.h>

namespace BikeShopTycoon {

/**
 * 音效管理器
 */
class AudioManager {
public:
    static AudioManager& Instance();
    
    bool Init();
    void Shutdown();
    
    // 音量控制
    void SetMasterVolume(float volume);  // 0.0 - 1.0
    void SetMusicVolume(float volume);
    void SetSFXVolume(float volume);
    
    // 播放
    void PlayMusic(const std::string& name, bool loop = true);
    void StopMusic();
    void PlaySFX(const std::string& name);
    
    // 预加载
    bool LoadSound(const std::string& name, const std::string& path);
    bool LoadMusic(const std::string& name, const std::string& path);
    
    // 便捷方法
    void PlayButtonClick();
    void PlayTransaction();
    void PlaySuccess();
    void PlayError();
    void PlayNotification();
    
private:
    AudioManager() = default;
    ~AudioManager();
    
    float masterVolume_ = 1.0f;
    float musicVolume_ = 0.7f;
    float sfxVolume_ = 0.8f;
    
    Mix_Music* currentMusic_ = nullptr;
    std::unordered_map<std::string, Mix_Chunk*> sounds_;
    std::unordered_map<std::string, Mix_Music*> music_;
    
    bool initialized_ = false;
};

} // namespace BikeShopTycoon