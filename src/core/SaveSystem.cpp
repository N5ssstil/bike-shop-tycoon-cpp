#include "SaveSystem.h"
#include "data/Types.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

namespace BikeShopTycoon {

SaveSystem& SaveSystem::Instance() {
    static SaveSystem instance;
    return instance;
}

bool SaveSystem::Save(const std::string& path, const PlayerData& data) {
    // 创建备份
    CreateBackup();
    
    // 构建JSON字符串
    std::ostringstream json;
    json << "{\n";
    json << "  \"saveVersion\": " << data.saveVersion << ",\n";
    json << "  \"lastSaveTime\": \"" << data.lastSaveTime << "\",\n";
    json << "  \"money\": " << data.money << ",\n";
    json << "  \"reputation\": " << data.reputation << ",\n";
    json << "  \"day\": " << data.day << ",\n";
    json << "  \"fansCount\": " << data.fansCount << ",\n";
    json << "  \"shopLevel\": " << data.shopLevel << ",\n";
    json << "  \"hasShowroom\": " << (data.hasShowroom ? "true" : "false") << ",\n";
    json << "  \"hasWorkshop\": " << (data.hasWorkshop ? "true" : "false") << ",\n";
    json << "  \"hasCustomStudio\": " << (data.hasCustomStudio ? "true" : "false") << ",\n";
    
    // 解锁品牌
    json << "  \"unlockedBrands\": [";
    for (size_t i = 0; i < data.unlockedBrands.size(); ++i) {
        json << "\"" << data.unlockedBrands[i] << "\"";
        if (i < data.unlockedBrands.size() - 1) json << ", ";
    }
    json << "],\n";
    
    // 已完成里程碑
    json << "  \"completedMilestones\": [";
    for (size_t i = 0; i < data.completedMilestones.size(); ++i) {
        json << "\"" << data.completedMilestones[i] << "\"";
        if (i < data.completedMilestones.size() - 1) json << ", ";
    }
    json << "]\n";
    
    json << "}\n";
    
    return WriteJson(path, json.str());
}

bool SaveSystem::Load(const std::string& path, PlayerData& data) {
    std::string json;
    if (!ReadJson(path, json)) {
        // 尝试从备份恢复
        return RestoreBackup(data);
    }
    
    // 简单解析JSON（手动解析，避免依赖第三方库）
    auto parseInt = [&json](const std::string& key) -> int {
        std::string search = "\"" + key + "\": ";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.length();
        
        std::string numStr;
        while (pos < json.length() && (isdigit(json[pos]) || json[pos] == '-')) {
            numStr += json[pos++];
        }
        return numStr.empty() ? 0 : std::stoi(numStr);
    };
    
    auto parseBool = [&json](const std::string& key) -> bool {
        std::string search = "\"" + key + "\": ";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return false;
        pos += search.length();
        return json.substr(pos, 4) == "true";
    };
    
    auto parseString = [&json](const std::string& key) -> std::string {
        std::string search = "\"" + key + "\": \"";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return "";
        pos += search.length();
        
        size_t end = json.find("\"", pos);
        return end != std::string::npos ? json.substr(pos, end - pos) : "";
    };
    
    auto parseStringArray = [&json](const std::string& key) -> std::vector<std::string> {
        std::vector<std::string> result;
        std::string search = "\"" + key + "\": [";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return result;
        pos += search.length();
        
        while (pos < json.length()) {
            // 跳过空白
            while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\n')) pos++;
            
            if (json[pos] == ']') break;
            if (json[pos] == '"') {
                pos++;
                size_t end = json.find("\"", pos);
                if (end != std::string::npos) {
                    result.push_back(json.substr(pos, end - pos));
                    pos = end + 1;
                }
            } else {
                pos++;
            }
        }
        return result;
    };
    
    // 解析数据
    data.saveVersion = parseInt("saveVersion");
    data.lastSaveTime = parseString("lastSaveTime");
    data.money = parseInt("money");
    data.reputation = parseInt("reputation");
    data.day = parseInt("day");
    data.fansCount = parseInt("fansCount");
    data.shopLevel = parseInt("shopLevel");
    data.hasShowroom = parseBool("hasShowroom");
    data.hasWorkshop = parseBool("hasWorkshop");
    data.hasCustomStudio = parseBool("hasCustomStudio");
    data.unlockedBrands = parseStringArray("unlockedBrands");
    data.completedMilestones = parseStringArray("completedMilestones");
    
    // 数据验证
    if (data.money < 0) data.money = 0;
    if (data.reputation < 0) data.reputation = 0;
    if (data.day < 1) data.day = 1;
    
    return true;
}

bool SaveSystem::SaveGame(const PlayerData& data) {
    // 更新保存时间
    auto now = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    PlayerData saveData = data;
    saveData.lastSaveTime = buf;
    
    return Save(savePath_, saveData);
}

bool SaveSystem::LoadGame(PlayerData& data) {
    return Load(savePath_, data);
}

bool SaveSystem::HasSave() const {
    std::ifstream file(savePath_);
    return file.good();
}

void SaveSystem::DeleteSave() {
    std::remove(savePath_.c_str());
    std::remove(backupPath_.c_str());
}

bool SaveSystem::GetSaveInfo(SaveInfo& info) {
    std::string json;
    if (!ReadJson(savePath_, json)) return false;
    
    auto parseInt = [&json](const std::string& key) -> int {
        std::string search = "\"" + key + "\": ";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.length();
        
        std::string numStr;
        while (pos < json.length() && (isdigit(json[pos]) || json[pos] == '-')) {
            numStr += json[pos++];
        }
        return numStr.empty() ? 0 : std::stoi(numStr);
    };
    
    auto parseString = [&json](const std::string& key) -> std::string {
        std::string search = "\"" + key + "\": \"";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return "";
        pos += search.length();
        
        size_t end = json.find("\"", pos);
        return end != std::string::npos ? json.substr(pos, end - pos) : "";
    };
    
    info.saveVersion = parseInt("saveVersion");
    info.lastSaveTime = parseString("lastSaveTime");
    info.day = parseInt("day");
    info.money = parseInt("money");
    info.reputation = parseInt("reputation");
    
    return true;
}

bool SaveSystem::WriteJson(const std::string& path, const std::string& json) {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    
    file << json;
    return true;
}

bool SaveSystem::ReadJson(const std::string& path, std::string& json) {
    std::ifstream file(path);
    if (!file.is_open()) return false;
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    json = buffer.str();
    return true;
}

void SaveSystem::CreateBackup() {
    std::ifstream src(savePath_, std::ios::binary);
    if (!src.is_open()) return;
    
    std::ofstream dst(backupPath_, std::ios::binary);
    dst << src.rdbuf();
}

bool SaveSystem::RestoreBackup(PlayerData& data) {
    std::ifstream file(backupPath_);
    if (!file.is_open()) return false;
    
    return Load(backupPath_, data);
}

} // namespace BikeShopTycoon