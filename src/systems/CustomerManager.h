#pragma once

#include "data/Types.h"
#include <vector>
#include <functional>
#include <random>
#include <memory>
#include <chrono>

namespace BikeShopTycoon {

/**
 * 顾客故事
 */
struct CustomerStory {
    std::string id;
    std::string title;
    std::string description;
    CustomerType targetCustomerType;
    int reputationBonus;
};

/**
 * 顾客生成设置
 */
struct CustomerGeneratorSettings {
    int studentWeight = 30;
    int commuterWeight = 35;
    int enthusiastWeight = 20;
    int racerWeight = 10;
    int influencerWeight = 5;
    
    float minSpawnInterval = 8.0f;
    float maxSpawnInterval = 20.0f;
    int maxConcurrentCustomers = 3;
};

/**
 * 顾客管理系统
 */
class CustomerManager {
public:
    using CustomerCallback = std::function<void(const Customer&)>;
    
    CustomerManager(PlayerData& playerData, const CustomerGeneratorSettings& settings = {});
    
    // 更新（每帧调用）
    void Update(float deltaTime);
    
    // 生成顾客
    Customer GenerateCustomer();
    
    // 获取活跃顾客
    const std::vector<Customer>& GetActiveCustomers() const { return activeCustomers_; }
    std::vector<Customer>& GetActiveCustomers() { return activeCustomers_; }
    int GetActiveCustomerCount() const { return static_cast<int>(activeCustomers_.size()); }
    
    // 根据索引获取顾客
    Customer* GetCustomerByIndex(int index);
    
    // 交互
    void InteractWithCustomer(Customer& customer, const std::string& dialogueChoice);
    
    // 交易
    bool CompleteTransaction(Customer& customer, const Item& item);
    void CustomerLeaveUnsatisfied(Customer& customer);
    
    // 匹配度计算
    int CalculateMatchScore(const Customer& customer, const Item& item) const;
    
    // 事件回调
    void SetOnCustomerEnter(CustomerCallback callback) { onCustomerEnter_ = callback; }
    void SetOnCustomerLeave(CustomerCallback callback) { onCustomerLeave_ = callback; }
    
private:
    CustomerType GetRandomCustomerType();
    void ConfigureCustomerByType(Customer& customer);
    void TryAssignStory(Customer& customer);
    std::string GenerateName();
    void LoadStories();
    
    PlayerData& playerData_;
    CustomerGeneratorSettings settings_;
    std::vector<Customer> activeCustomers_;
    std::vector<CustomerStory> storyDatabase_;
    std::mt19937 random_;
    
    float spawnTimer_ = 0.0f;
    float nextSpawnTime_ = 0.0f;
    
    CustomerCallback onCustomerEnter_;
    CustomerCallback onCustomerLeave_;
};

} // namespace BikeShopTycoon