#include "RepairService.h"
#include <algorithm>
#include <chrono>

namespace BikeShopTycoon {

RepairService::RepairService(PlayerData& playerData)
    : playerData_(playerData)
{
}

RepairJob RepairService::CreateJob(RepairType type, const Customer& customer) {
    RepairJob job;
    job.id = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    job.type = type;
    job.customer = customer;
    job.status = RepairJob::Status::Pending;
    job.baseCost = GetRepairCost(type);
    job.estimatedMinutes = GetRepairTime(type);
    job.requiredTools = GetRequiredTools(type);
    return job;
}

RepairResult RepairService::ExecuteRepair(RepairJob& job) {
    RepairResult result;
    result.job = job;
    result.success = true;
    
    // 计算收益
    int baseIncome = job.baseCost;
    int reputationGain = GetReputationGain(job.type);
    
    // 店铺有维修区时，效率提升
    if (playerData_.hasWorkshop) {
        baseIncome = static_cast<int>(baseIncome * 1.2f);
        reputationGain += 1;
    }
    
    result.income = baseIncome;
    result.reputationGain = reputationGain;
    
    // 更新玩家数据
    playerData_.money += baseIncome;
    playerData_.reputation += reputationGain;
    
    job.status = RepairJob::Status::Completed;
    
    if (onRepairComplete_) {
        onRepairComplete_(job);
    }
    
    return result;
}

int RepairService::GetRepairCost(RepairType type) const {
    switch (type) {
        case RepairType::FlatTire: return 50;
        case RepairType::GearAdjustment: return 80;
        case RepairType::BrakeService: return 60;
        case RepairType::WheelTruing: return 150;
        case RepairType::FullService: return 300;
        case RepairType::CustomTuning: return 500;
        default: return 50;
    }
}

int RepairService::GetRepairTime(RepairType type) const {
    switch (type) {
        case RepairType::FlatTire: return 15;
        case RepairType::GearAdjustment: return 30;
        case RepairType::BrakeService: return 20;
        case RepairType::WheelTruing: return 60;
        case RepairType::FullService: return 120;
        case RepairType::CustomTuning: return 90;
        default: return 30;
    }
}

std::vector<std::string> RepairService::GetRequiredTools(RepairType type) const {
    switch (type) {
        case RepairType::FlatTire:
            return {"撬胎棒", "打气筒", "备用内胎"};
        case RepairType::GearAdjustment:
            return {"内六角扳手", "螺丝刀", "变速调试架"};
        case RepairType::BrakeService:
            return {"内六角扳手", "刹车油", "注油器"};
        case RepairType::WheelTruing:
            return {"编圈台", "辐条扳手", "张力计"};
        case RepairType::FullService:
            return {"全套工具", "润滑脂", "清洁剂"};
        case RepairType::CustomTuning:
            return {"Fitting工具", "扭矩扳手", "专业测量仪"};
        default:
            return {"基础工具包"};
    }
}

int RepairService::GetReputationGain(RepairType type) const {
    switch (type) {
        case RepairType::FlatTire: return 1;
        case RepairType::GearAdjustment: return 2;
        case RepairType::BrakeService: return 1;
        case RepairType::WheelTruing: return 3;
        case RepairType::FullService: return 4;
        case RepairType::CustomTuning: return 5;
        default: return 1;
    }
}

} // namespace BikeShopTycoon