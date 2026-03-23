#include "ProductDatabase.h"
#include <algorithm>

namespace BikeShopTycoon {

ProductDatabase& ProductDatabase::Instance() {
    static ProductDatabase instance;
    return instance;
}

void ProductDatabase::Initialize() {
    CreateDefaultBrands();
    CreateDefaultProducts();
}

void ProductDatabase::CreateDefaultBrands() {
    brands_ = {
        {"brand_giant", "Giant (捷安特)", "台湾自行车品牌，全球最大的自行车制造商", 10, 0, 1.0f, 1.0f},
        {"brand_specialized", "Specialized (闪电)", "美国高端自行车品牌，环法冠军战车", 20, 5000, 1.1f, 1.2f},
        {"brand_trek", "Trek (崔克)", "美国自行车品牌，创新技术领导者", 15, 3000, 1.05f, 1.15f},
        {"brand_cannondale", "Cannondale (佳能戴尔)", "美国品牌，以铝合金车架闻名", 18, 4000, 1.08f, 1.18f},
        {"brand_pinarello", "Pinarello (皮纳瑞罗)", "意大利顶级品牌，环法冠军首选", 50, 50000, 1.2f, 1.5f}
    };
}

void ProductDatabase::CreateDefaultProducts() {
    // ============ 入门级公路车 ============
    Bike entry1;
    entry1.id = "bike_entry_001";
    entry1.name = "捷安特 Escape 3";
    entry1.description = "入门级铝合金公路车，适合新手通勤";
    entry1.type = ItemType::Bike;
    entry1.tier = ItemTier::Entry;
    entry1.brand = "Giant";
    entry1.purchasePrice = 1800;
    entry1.sellPrice = 2500;
    entry1.weight = 10500;
    entry1.durability = 80;
    entry1.performance = 40;
    entry1.requiredReputation = 0;
    entry1.frameMaterial = Bike::FrameMaterial::Aluminum;
    entry1.brakeType = Bike::BrakeType::RimBrake;
    entry1.gears = 16;
    entry1.groupsetBrand = "Shimano Claris";
    entry1.isForBeginners = true;
    entry1.isForCommuting = true;
    entry1.isForRacing = false;
    bikes_.push_back(entry1);
    items_.push_back(entry1);
    
    Bike entry2;
    entry2.id = "bike_entry_002";
    entry2.name = "美利达 挑战者 300";
    entry2.description = "入门山地车，适合轻度越野和通勤";
    entry2.type = ItemType::Bike;
    entry2.tier = ItemTier::Entry;
    entry2.brand = "Merida";
    entry2.purchasePrice = 2200;
    entry2.sellPrice = 3000;
    entry2.weight = 12000;
    entry2.durability = 90;
    entry2.performance = 35;
    entry2.requiredReputation = 0;
    entry2.frameMaterial = Bike::FrameMaterial::Aluminum;
    entry2.brakeType = Bike::BrakeType::DiscBrake;
    entry2.gears = 18;
    entry2.groupsetBrand = "Shimano Altus";
    entry2.isForBeginners = true;
    entry2.isForCommuting = true;
    entry2.isForRacing = false;
    bikes_.push_back(entry2);
    items_.push_back(entry2);
    
    // ============ 中端公路车 ============
    Bike mid1;
    mid1.id = "bike_mid_001";
    mid1.name = "捷安特 TCR Advanced 2";
    mid1.description = "碳纤维公路车，适合训练和业余比赛";
    mid1.type = ItemType::Bike;
    mid1.tier = ItemTier::Mid;
    mid1.brand = "Giant";
    mid1.purchasePrice = 12000;
    mid1.sellPrice = 15800;
    mid1.weight = 7800;
    mid1.durability = 75;
    mid1.performance = 75;
    mid1.requiredReputation = 20;
    mid1.frameMaterial = Bike::FrameMaterial::Carbon;
    mid1.brakeType = Bike::BrakeType::DiscBrake;
    mid1.gears = 22;
    mid1.groupsetBrand = "Shimano 105";
    mid1.isForBeginners = false;
    mid1.isForCommuting = false;
    mid1.isForRacing = true;
    bikes_.push_back(mid1);
    items_.push_back(mid1);
    
    Bike mid2;
    mid2.id = "bike_mid_002";
    mid2.name = "闪电 Allez Sprint";
    mid2.description = "铝合金竞技公路车，冲刺手的最爱";
    mid2.type = ItemType::Bike;
    mid2.tier = ItemTier::Mid;
    mid2.brand = "Specialized";
    mid2.purchasePrice = 15000;
    mid2.sellPrice = 19800;
    mid2.weight = 8200;
    mid2.durability = 80;
    mid2.performance = 80;
    mid2.requiredReputation = 30;
    mid2.frameMaterial = Bike::FrameMaterial::Aluminum;
    mid2.brakeType = Bike::BrakeType::DiscBrake;
    mid2.gears = 22;
    mid2.groupsetBrand = "Shimano 105";
    mid2.isForBeginners = false;
    mid2.isForCommuting = false;
    mid2.isForRacing = true;
    bikes_.push_back(mid2);
    items_.push_back(mid2);
    
    // ============ 高端公路车 ============
    Bike high1;
    high1.id = "bike_high_001";
    high1.name = "捷安特 Propel Advanced SL";
    high1.description = "顶级气动公路车，职业车手的选择";
    high1.type = ItemType::Bike;
    high1.tier = ItemTier::High;
    high1.brand = "Giant";
    high1.purchasePrice = 45000;
    high1.sellPrice = 58000;
    high1.weight = 6800;
    high1.durability = 70;
    high1.performance = 95;
    high1.requiredReputation = 100;
    high1.frameMaterial = Bike::FrameMaterial::Carbon;
    high1.brakeType = Bike::BrakeType::DiscBrake;
    high1.gears = 24;
    high1.groupsetBrand = "Shimano Dura-Ace";
    high1.isForBeginners = false;
    high1.isForCommuting = false;
    high1.isForRacing = true;
    bikes_.push_back(high1);
    items_.push_back(high1);
    
    Bike high2;
    high2.id = "bike_high_002";
    high2.name = "闪电 Tarmac SL7";
    high2.description = "环法冠军战车，全能竞赛公路车";
    high2.type = ItemType::Bike;
    high2.tier = ItemTier::High;
    high2.brand = "Specialized";
    high2.purchasePrice = 55000;
    high2.sellPrice = 72000;
    high2.weight = 6600;
    high2.durability = 68;
    high2.performance = 98;
    high2.requiredReputation = 150;
    high2.frameMaterial = Bike::FrameMaterial::Carbon;
    high2.brakeType = Bike::BrakeType::DiscBrake;
    high2.gears = 24;
    high2.groupsetBrand = "SRAM Red";
    high2.isForBeginners = false;
    high2.isForCommuting = false;
    high2.isForRacing = true;
    bikes_.push_back(high2);
    items_.push_back(high2);
    
    // ============ 配件 ============
    Item accessory1;
    accessory1.id = "acc_helmet_001";
    accessory1.name = "Giro Aether MIPS";
    accessory1.description = "高端公路头盔，MIPS安全系统";
    accessory1.type = ItemType::Accessories;
    accessory1.tier = ItemTier::High;
    accessory1.brand = "Giro";
    accessory1.purchasePrice = 1800;
    accessory1.sellPrice = 2500;
    accessory1.weight = 280;
    accessory1.durability = 100;
    accessory1.performance = 90;
    accessory1.requiredReputation = 10;
    items_.push_back(accessory1);
    
    Item accessory2;
    accessory2.id = "acc_computer_001";
    accessory2.name = "Garmin Edge 540";
    accessory2.description = "专业骑行电脑，GPS导航";
    accessory2.type = ItemType::Accessories;
    accessory2.tier = ItemTier::Mid;
    accessory2.brand = "Garmin";
    accessory2.purchasePrice = 2800;
    accessory2.sellPrice = 3500;
    accessory2.weight = 85;
    accessory2.durability = 100;
    accessory2.performance = 85;
    accessory2.requiredReputation = 5;
    items_.push_back(accessory2);
}

const Item* ProductDatabase::GetItemById(const std::string& id) const {
    auto it = std::find_if(items_.begin(), items_.end(),
        [&](const Item& item) { return item.id == id; });
    return it != items_.end() ? &(*it) : nullptr;
}

std::vector<Item> ProductDatabase::GetItemsByTier(ItemTier tier) const {
    std::vector<Item> result;
    for (const auto& item : items_) {
        if (item.tier == tier) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Item> ProductDatabase::GetItemsByType(ItemType type) const {
    std::vector<Item> result;
    for (const auto& item : items_) {
        if (item.type == type) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Item> ProductDatabase::GetItemsByPriceRange(int minPrice, int maxPrice) const {
    std::vector<Item> result;
    for (const auto& item : items_) {
        if (item.sellPrice >= minPrice && item.sellPrice <= maxPrice) {
            result.push_back(item);
        }
    }
    return result;
}

const Bike* ProductDatabase::GetBikeById(const std::string& id) const {
    auto it = std::find_if(bikes_.begin(), bikes_.end(),
        [&](const Bike& bike) { return bike.id == id; });
    return it != bikes_.end() ? &(*it) : nullptr;
}

const ProductDatabase::BrandData* ProductDatabase::GetBrandById(const std::string& id) const {
    auto it = std::find_if(brands_.begin(), brands_.end(),
        [&](const BrandData& brand) { return brand.id == id; });
    return it != brands_.end() ? &(*it) : nullptr;
}

} // namespace BikeShopTycoon