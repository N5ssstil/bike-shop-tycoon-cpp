#pragma once

#include <string>
#include <memory>

namespace BikeShopTycoon {

struct PlayerData;

/**
 * 存档系统 - JSON序列化
 */
class SaveSystem {
public:
    static SaveSystem& Instance();
    
    // 保存/加载
    bool Save(const std::string& path, const PlayerData& data);
    bool Load(const std::string& path, PlayerData& data);
    
    // 快捷方法
    bool SaveGame(const PlayerData& data);
    bool LoadGame(PlayerData& data);
    bool HasSave() const;
    void DeleteSave();
    
    // 存档信息
    struct SaveInfo {
        int saveVersion;
        std::string lastSaveTime;
        int day;
        int money;
        int reputation;
    };
    
    bool GetSaveInfo(SaveInfo& info);
    
private:
    SaveSystem() = default;
    
    std::string savePath_ = "save.json";
    std::string backupPath_ = "save_backup.json";
    
    bool WriteJson(const std::string& path, const std::string& json);
    bool ReadJson(const std::string& path, std::string& json);
    void CreateBackup();
    bool RestoreBackup(PlayerData& data);
};

} // namespace BikeShopTycoon