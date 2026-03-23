#pragma once

#include "data/Types.h"
#include <vector>
#include <functional>

namespace BikeShopTycoon {

/**
 * 库存数据
 */
struct InventoryData {
    std::vector<InventoryItem> items;
    int maxCapacity = 50;
    int usedCapacity = 0;
    
    bool CanAddItem(int quantity = 1) const {
        return usedCapacity + quantity <= maxCapacity;
    }
    
    void AddItem(const Item& item, int quantity, int purchasePrice);
    bool RemoveItem(const std::string& itemId, int quantity);
    InventoryItem* GetItem(const std::string& itemId);
    int GetTotalValue() const;
};

/**
 * 库存管理器
 */
class InventoryManager {
public:
    using ItemCallback = std::function<void(const InventoryItem&, int)>;
    
    InventoryManager(PlayerData& playerData);
    
    // 进货
    bool PurchaseStock(const Item& item, int quantity, int unitPrice);
    
    // 销售
    bool SellItem(const std::string& itemId, int quantity);
    
    // 获取库存
    InventoryData& GetInventory() { return inventory_; }
    const InventoryData& GetInventory() const { return inventory_; }
    
    // 事件回调
    void SetOnItemSold(ItemCallback callback) { onItemSold_ = callback; }
    
private:
    int CalculateReputationGain(const Item& item) const;
    
    PlayerData& playerData_;
    InventoryData inventory_;
    ItemCallback onItemSold_;
};

} // namespace BikeShopTycoon