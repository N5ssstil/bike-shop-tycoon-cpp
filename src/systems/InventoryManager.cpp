#include "InventoryManager.h"
#include <algorithm>

namespace BikeShopTycoon {

// InventoryData 实现
void InventoryData::AddItem(const Item& item, int quantity, int purchasePrice) {
    auto it = std::find_if(items.begin(), items.end(),
        [&](const InventoryItem& i) { return i.itemId == item.id; });
    
    if (it != items.end()) {
        it->quantity += quantity;
        it->purchasePrice = purchasePrice;
    } else {
        InventoryItem newItem;
        newItem.itemId = item.id;
        newItem.itemData = item;
        newItem.quantity = quantity;
        newItem.purchasePrice = purchasePrice;
        newItem.daysInStock = 0;
        items.push_back(newItem);
    }
    
    usedCapacity += quantity;
}

bool InventoryData::RemoveItem(const std::string& itemId, int quantity) {
    auto it = std::find_if(items.begin(), items.end(),
        [&](const InventoryItem& i) { return i.itemId == itemId; });
    
    if (it == items.end() || it->quantity < quantity) {
        return false;
    }
    
    it->quantity -= quantity;
    usedCapacity -= quantity;
    
    if (it->quantity <= 0) {
        items.erase(it);
    }
    
    return true;
}

InventoryItem* InventoryData::GetItem(const std::string& itemId) {
    auto it = std::find_if(items.begin(), items.end(),
        [&](const InventoryItem& i) { return i.itemId == itemId; });
    return it != items.end() ? &(*it) : nullptr;
}

int InventoryData::GetTotalValue() const {
    int total = 0;
    for (const auto& item : items) {
        total += item.purchasePrice * item.quantity;
    }
    return total;
}

// InventoryManager 实现
InventoryManager::InventoryManager(PlayerData& playerData)
    : playerData_(playerData)
{
}

bool InventoryManager::PurchaseStock(const Item& item, int quantity, int unitPrice) {
    int totalCost = unitPrice * quantity;
    
    // 检查资金
    if (playerData_.money < totalCost) {
        return false;
    }
    
    // 检查库存容量
    if (!inventory_.CanAddItem(quantity)) {
        return false;
    }
    
    // 扣款并入库
    playerData_.money -= totalCost;
    inventory_.AddItem(item, quantity, unitPrice);
    
    return true;
}

bool InventoryManager::SellItem(const std::string& itemId, int quantity) {
    auto invItem = inventory_.GetItem(itemId);
    if (!invItem || invItem->quantity < quantity) {
        return false;
    }
    
    int sellPrice = invItem->itemData.sellPrice * quantity;
    
    // 增加金钱和口碑
    playerData_.money += sellPrice;
    playerData_.reputation += CalculateReputationGain(invItem->itemData);
    
    // 减少库存
    inventory_.RemoveItem(itemId, quantity);
    
    if (onItemSold_) {
        onItemSold_(*invItem, quantity);
    }
    
    return true;
}

int InventoryManager::CalculateReputationGain(const Item& item) const {
    switch (item.tier) {
        case ItemTier::Entry: return 1;
        case ItemTier::Mid: return 2;
        case ItemTier::High: return 3;
        case ItemTier::Pro: return 5;
        default: return 1;
    }
}

} // namespace BikeShopTycoon