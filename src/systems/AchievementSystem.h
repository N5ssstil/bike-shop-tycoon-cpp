#pragma once

#include "data/Types.h"
#include <vector>
#include <functional>

namespace BikeShopTycoon {

/**
 * 成就定义
 */
struct Achievement {
    std::string id;
    std::string name;
    std::string description;
    
    enum class Type {
        Money,
        Reputation,
        Day,
        BrandUnlock,
        Fans
    } type;
    
    int requiredValue;
    int rewardMoney = 0;
    int rewardReputation = 0;
};

/**
 * 成就系统
 */
class AchievementSystem {
public:
    using AchievementCallback = std::function<void(const Achievement&)>;
    
    AchievementSystem(PlayerData& playerData);
    
    // 检查成就
    void CheckAchievements();
    
    // 获取所有成就状态
    struct AchievementStatus {
        Achievement achievement;
        bool isUnlocked;
        float progress;
    };
    
    std::vector<AchievementStatus> GetAchievementStatuses() const;
    
    // 回调
    void SetOnAchievementUnlocked(AchievementCallback callback) { onUnlocked_ = callback; }
    
private:
    bool CheckCondition(const Achievement& achievement) const;
    float GetProgress(const Achievement& achievement) const;
    void UnlockAchievement(const Achievement& achievement);
    void LoadAchievements();
    
    PlayerData& playerData_;
    std::vector<Achievement> achievements_;
    AchievementCallback onUnlocked_;
};

} // namespace BikeShopTycoon