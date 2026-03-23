#pragma once

#include <string>
#include <memory>

namespace BikeShopTycoon {

/**
 * 存档系统
 */
class SaveSystem {
public:
    static SaveSystem& Instance();
    
    bool Save(const std::string& path);
    bool Load(const std::string& path);
    bool HasSave() const;
    void DeleteSave();
    
private:
    SaveSystem() = default;
    std::string savePath_ = "save.json";
};

} // namespace BikeShopTycoon