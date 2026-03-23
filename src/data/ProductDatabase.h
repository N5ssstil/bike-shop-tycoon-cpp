#pragma once

#include "data/Types.h"
#include <vector>
#include <string>
#include <memory>

namespace BikeShopTycoon {

/**
 * 商品数据库
 */
class ProductDatabase {
public:
    static ProductDatabase& Instance();
    
    // 初始化
    void Initialize();
    
    // 获取商品
    const std::vector<Item>& GetAllItems() const { return items_; }
    const Item* GetItemById(const std::string& id) const;
    std::vector<Item> GetItemsByTier(ItemTier tier) const;
    std::vector<Item> GetItemsByType(ItemType type) const;
    std::vector<Item> GetItemsByPriceRange(int minPrice, int maxPrice) const;
    
    // 自行车专用
    const std::vector<Bike>& GetAllBikes() const { return bikes_; }
    const Bike* GetBikeById(const std::string& id) const;
    
    // 品牌数据
    struct BrandData {
        std::string id;
        std::string name;
        std::string description;
        int baseReputation = 10;
        int unlockCost = 0;
        float qualityMultiplier = 1.0f;
        float priceMultiplier = 1.0f;
    };
    
    const std::vector<BrandData>& GetAllBrands() const { return brands_; }
    const BrandData* GetBrandById(const std::string& id) const;
    
private:
    ProductDatabase() = default;
    
    void CreateDefaultProducts();
    void CreateDefaultBrands();
    
    std::vector<Item> items_;
    std::vector<Bike> bikes_;
    std::vector<BrandData> brands_;
};

} // namespace BikeShopTycoon