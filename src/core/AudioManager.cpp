#include "AudioManager.h"

namespace BikeShopTycoon {

AudioManager& AudioManager::Instance() {
    static AudioManager instance;
    return instance;
}

AudioManager::~AudioManager() {
    Shutdown();
}

bool AudioManager::Init() {
    if (initialized_) return true;
    
    // 初始化 SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        return false;
    }
    
    initialized_ = true;
    return true;
}

void AudioManager::Shutdown() {
    if (!initialized_) return;
    
    // 释放所有音效
    for (auto& pair : sounds_) {
        if (pair.second) {
            Mix_FreeChunk(pair.second);
        }
    }
    sounds_.clear();
    
    // 释放所有音乐
    for (auto& pair : music_) {
        if (pair.second) {
            Mix_FreeMusic(pair.second);
        }
    }
    music_.clear();
    
    Mix_CloseAudio();
    initialized_ = false;
}

void AudioManager::SetMasterVolume(float volume) {
    masterVolume_ = volume;
    Mix_Volume(-1, (int)(sfxVolume_ * masterVolume_ * MIX_MAX_VOLUME));
    Mix_VolumeMusic((int)(musicVolume_ * masterVolume_ * MIX_MAX_VOLUME));
}

void AudioManager::SetMusicVolume(float volume) {
    musicVolume_ = volume;
    Mix_VolumeMusic((int)(musicVolume_ * masterVolume_ * MIX_MAX_VOLUME));
}

void AudioManager::SetSFXVolume(float volume) {
    sfxVolume_ = volume;
    Mix_Volume(-1, (int)(sfxVolume_ * masterVolume_ * MIX_MAX_VOLUME));
}

bool AudioManager::LoadSound(const std::string& name, const std::string& path) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) return false;
    
    sounds_[name] = chunk;
    return true;
}

bool AudioManager::LoadMusic(const std::string& name, const std::string& path) {
    Mix_Music* mus = Mix_LoadMUS(path.c_str());
    if (!mus) return false;
    
    music_[name] = mus;
    return true;
}

void AudioManager::PlayMusic(const std::string& name, bool loop) {
    auto it = music_.find(name);
    if (it == music_.end()) return;
    
    if (currentMusic_) {
        Mix_HaltMusic();
    }
    
    currentMusic_ = it->second;
    Mix_PlayMusic(currentMusic_, loop ? -1 : 0);
}

void AudioManager::StopMusic() {
    Mix_HaltMusic();
    currentMusic_ = nullptr;
}

void AudioManager::PlaySFX(const std::string& name) {
    auto it = sounds_.find(name);
    if (it == sounds_.end()) return;
    
    Mix_PlayChannel(-1, it->second, 0);
}

void AudioManager::PlayButtonClick() {
    PlaySFX("button_click");
}

void AudioManager::PlayTransaction() {
    PlaySFX("transaction");
}

void AudioManager::PlaySuccess() {
    PlaySFX("success");
}

void AudioManager::PlayError() {
    PlaySFX("error");
}

void AudioManager::PlayNotification() {
    PlaySFX("notification");
}

} // namespace BikeShopTycoon