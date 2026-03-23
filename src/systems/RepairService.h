#pragma once

#include "data/Types.h"
#include <string>
#include <vector>
#include <functional>

namespace BikeShopTycoon {

/**
 * 维修工单
 */
struct RepairJob {
    std::string id;
    RepairType type;
    Customer customer;
    enum class Status {
        Pending,
        InProgress,
        Completed,
        Cancelled
    } status = Status::Pending;
    
    int baseCost;
    int estimatedMinutes;
    std::vector<std::string> requiredTools;
};

/**
 * 维修结果
 */
struct RepairResult {
    RepairJob job;
    bool success = false;
    int income = 0;
    int reputationGain = 0;
    std::string message;
};

/**
 * 维修服务
 */
class RepairService {
public:
    using JobCallback = std::function<void(const RepairJob&)>;
    
    RepairService(PlayerData& playerData);
    
    // 创建工单
    RepairJob CreateJob(RepairType type, const Customer& customer);
    
    // 执行维修
    RepairResult ExecuteRepair(RepairJob& job);
    
    // 获取维修费用/时间
    int GetRepairCost(RepairType type) const;
    int GetRepairTime(RepairType type) const;
    std::vector<std::string> GetRequiredTools(RepairType type) const;
    
    // 事件回调
    void SetOnRepairComplete(JobCallback callback) { onRepairComplete_ = callback; }
    
private:
    int GetReputationGain(RepairType type) const;
    
    PlayerData& playerData_;
    JobCallback onRepairComplete_;
};

} // namespace BikeShopTycoon