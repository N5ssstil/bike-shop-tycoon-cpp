#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace BikeShopTycoon {

/**
 * 玩家数据
 */
struct PlayerData {
    int32_t money = 10000;          // 资金
    int32_t reputation = 10;        // 口碑
    int32_t day = 1;                // 游戏天数
    int32_t fansCount = 0;          // 粉丝数
    
    // 店铺状态
    int32_t shopLevel = 1;
    bool hasShowroom = false;
    bool hasWorkshop = false;
    bool hasCustomStudio = false;
    
    // 解锁状态
    std::vector<std::string> unlockedBrands;
    std::vector<std::string> unlockedItems;
    std::vector<std::string> completedMilestones;
    
    // 存档信息
    int32_t saveVersion = 1;
    std::string lastSaveTime;
};

/**
 * 商品类型
 */
enum class ItemType {
    Bike,           // 整车
    Frame,          // 车架
    Groupset,       // 套件
    Wheels,         // 轮组
    Accessories,    // 配件
    Tools,          // 工具
    Apparel         // 骑行服
};

/**
 * 商品等级
 */
enum class ItemTier {
    Entry,          // 入门
    Mid,            // 中端
    High,           // 高端
    Pro             // 职业
};

/**
 * 商品数据
 */
struct Item {
    std::string id;
    std::string name;
    std::string description;
    ItemType type;
    ItemTier tier;
    std::string brand;
    
    int32_t purchasePrice;  // 进价
    int32_t sellPrice;      // 售价
    int32_t weight;         // 重量(克)
    int32_t durability;     // 耐久度
    int32_t performance;    // 性能值
    
    int32_t requiredReputation = 0;
};

/**
 * 自行车整车数据
 */
struct Bike : public Item {
    enum class FrameMaterial {
        Aluminum,
        Steel,
        Carbon,
        Titanium
    };
    
    enum class BrakeType {
        RimBrake,
        DiscBrake
    };
    
    FrameMaterial frameMaterial;
    BrakeType brakeType;
    int32_t gears;
    std::string groupsetBrand;
    
    bool isForRacing = false;
    bool isForCommuting = false;
    bool isForBeginners = false;
};

/**
 * 顾客类型
 */
enum class CustomerType {
    Student,
    Commuter,
    CyclingEnthusiast,
    Racer,
    Influencer
};

/**
 * 顾客状态
 */
enum class CustomerState {
    Entering,
    Browsing,
    Asking,
    Deciding,
    Purchasing,
    Leaving,
    Satisfied,
    Unsatisfied
};

/**
 * 顾客需求
 */
struct CustomerNeeds {
    bool needForRacing = false;
    bool needForCommuting = false;
    bool needForTraining = false;
    bool needForBeginners = false;
    bool needHighVisual = false;
    
    ItemTier preferredTier = ItemTier::Entry;
    std::string preferredBrand;
    std::string preferredColor;
};

/**
 * 顾客数据
 */
struct Customer {
    std::string id;
    std::string name;
    CustomerType type;
    CustomerState state = CustomerState::Entering;
    
    CustomerNeeds needs;
    int32_t budget;
    int32_t patience = 100;
    int32_t satisfaction = 50;
    
    std::string storyId;
    bool storyRevealed = false;
};

/**
 * 维修类型
 */
enum class RepairType {
    FlatTire,
    GearAdjustment,
    BrakeService,
    WheelTruing,
    FullService,
    CustomTuning
};

/**
 * 库存项
 */
struct InventoryItem {
    std::string itemId;
    Item itemData;
    int32_t quantity;
    int32_t daysInStock;
    int32_t purchasePrice;
};

/**
 * 事件选择
 */
struct EventChoice {
    std::string text;
    int32_t moneyChange = 0;
    int32_t reputationChange = 0;
    std::string unlockBrand;
};

/**
 * 事件数据
 */
struct GameEvent {
    std::string id;
    std::string title;
    std::string description;
    
    int32_t minDay = 1;
    int32_t minReputation = 0;
    int32_t maxReputation = 0;
    std::vector<EventChoice> choices;
};

} // namespace BikeShopTycoon