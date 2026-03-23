#include "AchievementSystem.h"
#include <algorithm>

namespace BikeShopTycoon {

AchievementSystem::AchievementSystem(PlayerData& playerData)
    : playerData_(playerData)
{
    LoadAchievements();
}

void AchievementSystem::CheckAchievements() {
    for (const auto& achievement : achievements_) {
        // 检查是否已解锁
        bool alreadyUnlocked = std::find(
            playerData_.completedMilestones.begin(),
            playerData_.completedMilestones.end(),
            achievement.id
        ) != playerData_.completedMilestones.end();
        
        if (!alreadyUnlocked && CheckCondition(achievement)) {
            UnlockAchievement(achievement);
        }
    }
}

bool AchievementSystem::CheckCondition(const Achievement& achievement) const {
    switch (achievement.type) {
        case Achievement::Type::Money:
            return playerData_.money >= achievement.requiredValue;
        case Achievement::Type::Reputation:
            return playerData_.reputation >= achievement.requiredValue;
        case Achievement::Type::Day:
            return playerData_.day >= achievement.requiredValue;
        case Achievement::Type::BrandUnlock:
            return (int)playerData_.unlockedBrands.size() >= achievement.requiredValue;
        case Achievement::Type::Fans:
            return playerData_.fansCount >= achievement.requiredValue;
    }
    return false;
}

float AchievementSystem::GetProgress(const Achievement& achievement) const {
    int current = 0;
    switch (achievement.type) {
        case Achievement::Type::Money:
            current = playerData_.money;
            break;
        case Achievement::Type::Reputation:
            current = playerData_.reputation;
            break;
        case Achievement::Type::Day:
            current = playerData_.day;
            break;
        case Achievement::Type::BrandUnlock:
            current = (int)playerData_.unlockedBrands.size();
            break;
        case Achievement::Type::Fans:
            current = playerData_.fansCount;
            break;
    }
    return std::min(1.0f, (float)current / achievement.requiredValue);
}

void AchievementSystem::UnlockAchievement(const Achievement& achievement) {
    playerData_.completedMilestones.push_back(achievement.id);
    
    // 发放奖励
    if (achievement.rewardMoney > 0) {
        playerData_.money += achievement.rewardMoney;
    }
    if (achievement.rewardReputation > 0) {
        playerData_.reputation += achievement.rewardReputation;
    }
    
    if (onUnlocked_) {
        onUnlocked_(achievement);
    }
}

std::vector<AchievementSystem::AchievementStatus> AchievementSystem::GetAchievementStatuses() const {
    std::vector<AchievementStatus> statuses;
    
    for (const auto& achievement : achievements_) {
        AchievementStatus status;
        status.achievement = achievement;
        status.isUnlocked = std::find(
            playerData_.completedMilestones.begin(),
            playerData_.completedMilestones.end(),
            achievement.id
        ) != playerData_.completedMilestones.end();
        status.progress = GetProgress(achievement);
        statuses.push_back(status);
    }
    
    return statuses;
}

void AchievementSystem::LoadAchievements() {
    achievements_ = {
        // 财富成就
        {"money_10k", "小本经营", "累计资金达到 1 万元", Achievement::Type::Money, 10000, 500},
        {"money_50k", "生意兴隆", "累计资金达到 5 万元", Achievement::Type::Money, 50000, 2000, 10},
        {"money_100k", "财源广进", "累计资金达到 10 万元", Achievement::Type::Money, 100000, 5000, 20},
        {"money_1m", "百万富翁", "累计资金达到 100 万元", Achievement::Type::Money, 1000000, 50000, 100},
        
        // 口碑成就
        {"rep_50", "小有名气", "口碑达到 50", Achievement::Type::Reputation, 50, 0, 5},
        {"rep_200", "声名远扬", "口碑达到 200", Achievement::Type::Reputation, 200, 1000, 20},
        {"rep_500", "行业标杆", "口碑达到 500", Achievement::Type::Reputation, 500, 5000, 50},
        {"rep_1000", "传奇车店", "口碑达到 1000", Achievement::Type::Reputation, 1000, 20000, 100},
        
        // 时间成就
        {"day_10", "坚持一周", "经营满 10 天", Achievement::Type::Day, 10, 500},
        {"day_30", "月度之星", "经营满 30 天", Achievement::Type::Day, 30, 2000, 10},
        {"day_100", "百年老店", "经营满 100 天", Achievement::Type::Day, 100, 10000, 30},
        
        // 品牌成就
        {"brand_3", "品牌合作", "解锁 3 个品牌授权", Achievement::Type::BrandUnlock, 3, 1000, 10},
        {"brand_5", "品牌矩阵", "解锁 5 个品牌授权", Achievement::Type::BrandUnlock, 5, 5000, 20},
        
        // 粉丝成就
        {"fans_100", "网络达人", "累计粉丝达到 100 人", Achievement::Type::Fans, 100, 500, 5},
        {"fans_500", "网红车店", "累计粉丝达到 500 人", Achievement::Type::Fans, 500, 2000, 15}
    };
}

} // namespace BikeShopTycoon