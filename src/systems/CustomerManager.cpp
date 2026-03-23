#include "CustomerManager.h"
#include <algorithm>

namespace BikeShopTycoon {

CustomerManager::CustomerManager(PlayerData& playerData, const CustomerGeneratorSettings& settings)
    : playerData_(playerData)
    , settings_(settings)
    , random_(std::random_device{}())
{
    LoadStories();
    
    // 初始化第一次生成时间
    std::uniform_real_distribution<float> dist(settings_.minSpawnInterval, settings_.maxSpawnInterval);
    nextSpawnTime_ = dist(random_);
}

void CustomerManager::Update(float deltaTime) {
    // 更新生成计时器
    spawnTimer_ += deltaTime;
    
    // 检查是否需要生成新顾客
    if (spawnTimer_ >= nextSpawnTime_ && GetActiveCustomerCount() < settings_.maxConcurrentCustomers) {
        GenerateCustomer();
        spawnTimer_ = 0.0f;
        
        // 设置下一次生成时间
        std::uniform_real_distribution<float> dist(settings_.minSpawnInterval, settings_.maxSpawnInterval);
        nextSpawnTime_ = dist(random_);
    }
    
    // 更新顾客状态（耐心值等）
    for (auto& customer : activeCustomers_) {
        if (customer.patience > 0) {
            customer.patience -= static_cast<int>(deltaTime * 2); // 每秒减少2点耐心
        }
        
        // 耐心耗尽则离开
        if (customer.patience <= 0 && customer.state != CustomerState::Leaving) {
            customer.state = CustomerState::Leaving;
        }
    }
}

Customer* CustomerManager::GetCustomerByIndex(int index) {
    if (index >= 0 && index < static_cast<int>(activeCustomers_.size())) {
        return &activeCustomers_[index];
    }
    return nullptr;
}

Customer CustomerManager::GenerateCustomer() {
    Customer customer;
    customer.id = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    customer.name = GenerateName();
    customer.type = GetRandomCustomerType();
    customer.state = CustomerState::Entering;
    
    ConfigureCustomerByType(customer);
    TryAssignStory(customer);
    
    activeCustomers_.push_back(customer);
    
    if (onCustomerEnter_) {
        onCustomerEnter_(customer);
    }
    
    return customer;
}

void CustomerManager::InteractWithCustomer(Customer& customer, const std::string& dialogueChoice) {
    customer.state = CustomerState::Asking;
    
    if (dialogueChoice == "recommend") {
        customer.state = CustomerState::Deciding;
    } else if (dialogueChoice == "repair") {
        customer.state = CustomerState::Deciding;
    }
}

bool CustomerManager::CompleteTransaction(Customer& customer, const Item& item) {
    if (customer.state != CustomerState::Purchasing) {
        return false;
    }
    
    customer.state = CustomerState::Satisfied;
    customer.satisfaction += 10;
    
    // 触发故事
    if (!customer.storyId.empty() && !customer.storyRevealed) {
        auto it = std::find_if(storyDatabase_.begin(), storyDatabase_.end(),
            [&](const CustomerStory& s) { return s.id == customer.storyId; });
        
        if (it != storyDatabase_.end()) {
            customer.storyRevealed = true;
            playerData_.reputation += it->reputationBonus;
        }
    }
    
    // 网红顾客带来粉丝
    if (customer.type == CustomerType::Influencer) {
        playerData_.fansCount += 50;
    }
    
    // 移除顾客
    activeCustomers_.erase(
        std::remove_if(activeCustomers_.begin(), activeCustomers_.end(),
            [&](const Customer& c) { return c.id == customer.id; }),
        activeCustomers_.end()
    );
    
    if (onCustomerLeave_) {
        onCustomerLeave_(customer);
    }
    
    return true;
}

void CustomerManager::CustomerLeaveUnsatisfied(Customer& customer) {
    customer.state = CustomerState::Unsatisfied;
    playerData_.reputation = std::max(0, playerData_.reputation - 5);
    
    activeCustomers_.erase(
        std::remove_if(activeCustomers_.begin(), activeCustomers_.end(),
            [&](const Customer& c) { return c.id == customer.id; }),
        activeCustomers_.end()
    );
    
    if (onCustomerLeave_) {
        onCustomerLeave_(customer);
    }
}

int CustomerManager::CalculateMatchScore(const Customer& customer, const Item& item) const {
    int score = 50; // 基础分
    
    // 检查等级偏好
    if (item.tier == customer.needs.preferredTier) {
        score += 15;
    }
    
    // 检查品牌偏好
    if (!customer.needs.preferredBrand.empty() && item.brand == customer.needs.preferredBrand) {
        score += 10;
    }
    
    return std::min(100, std::max(0, score));
}

CustomerType CustomerManager::GetRandomCustomerType() {
    int totalWeight = settings_.studentWeight + settings_.commuterWeight +
                      settings_.enthusiastWeight + settings_.racerWeight +
                      settings_.influencerWeight;
    
    std::uniform_int_distribution<int> dist(0, totalWeight - 1);
    int roll = dist(random_);
    
    if (roll < settings_.studentWeight) return CustomerType::Student;
    roll -= settings_.studentWeight;
    
    if (roll < settings_.commuterWeight) return CustomerType::Commuter;
    roll -= settings_.commuterWeight;
    
    if (roll < settings_.enthusiastWeight) return CustomerType::CyclingEnthusiast;
    roll -= settings_.enthusiastWeight;
    
    if (roll < settings_.racerWeight) return CustomerType::Racer;
    
    return CustomerType::Influencer;
}

void CustomerManager::ConfigureCustomerByType(Customer& customer) {
    std::uniform_int_distribution<int> budgetDist;
    
    switch (customer.type) {
        case CustomerType::Student:
            budgetDist = std::uniform_int_distribution<int>(1500, 5000);
            customer.budget = budgetDist(random_);
            customer.needs.needForBeginners = true;
            customer.needs.preferredTier = ItemTier::Entry;
            break;
            
        case CustomerType::Commuter:
            budgetDist = std::uniform_int_distribution<int>(2000, 8000);
            customer.budget = budgetDist(random_);
            customer.needs.needForCommuting = true;
            break;
            
        case CustomerType::CyclingEnthusiast:
            budgetDist = std::uniform_int_distribution<int>(5000, 30000);
            customer.budget = budgetDist(random_);
            customer.needs.needForTraining = true;
            customer.needs.preferredTier = ItemTier::Mid;
            break;
            
        case CustomerType::Racer:
            budgetDist = std::uniform_int_distribution<int>(20000, 100000);
            customer.budget = budgetDist(random_);
            customer.needs.needForRacing = true;
            customer.needs.preferredTier = ItemTier::Pro;
            break;
            
        case CustomerType::Influencer:
            budgetDist = std::uniform_int_distribution<int>(8000, 50000);
            customer.budget = budgetDist(random_);
            customer.needs.needHighVisual = true;
            // 随机颜色
            std::vector<std::string> colors = {"白色", "黑色", "红色", "蓝色", "黄色"};
            std::uniform_int_distribution<int> colorDist(0, colors.size() - 1);
            customer.needs.preferredColor = colors[colorDist(random_)];
            break;
    }
}

void CustomerManager::TryAssignStory(Customer& customer) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    if (dist(random_) > 0.3f) return; // 30% 概率
    
    std::vector<const CustomerStory*> matchingStories;
    for (const auto& story : storyDatabase_) {
        if (story.targetCustomerType == customer.type) {
            matchingStories.push_back(&story);
        }
    }
    
    if (!matchingStories.empty()) {
        std::uniform_int_distribution<int> storyDist(0, matchingStories.size() - 1);
        customer.storyId = matchingStories[storyDist(random_)]->id;
    }
}

std::string CustomerManager::GenerateName() {
    static const std::vector<std::string> firstNames = {
        "张", "李", "王", "刘", "陈", "杨", "赵", "黄", "周", "吴"
    };
    static const std::vector<std::string> lastNames = {
        "伟", "芳", "娜", "秀英", "敏", "静", "丽", "强", "磊", "军"
    };
    
    std::uniform_int_distribution<int> firstDist(0, firstNames.size() - 1);
    std::uniform_int_distribution<int> lastDist(0, lastNames.size() - 1);
    
    return firstNames[firstDist(random_)] + lastNames[lastDist(random_)];
}

void CustomerManager::LoadStories() {
    storyDatabase_ = {
        {"story_student_001", "校园赛的梦想", 
         "一名学生攒了很久的钱，想买第一台公路车备战校园赛",
         CustomerType::Student, 5},
         
        {"story_commuter_001", "为了多陪孩子",
         "一位通勤族想换一辆更轻便的车，每天能多陪孩子半小时",
         CustomerType::Commuter, 3},
         
        {"story_enthusiast_001", "骑游的故事",
         "一位骑行爱好者来修车，分享了他的川藏线骑行经历",
         CustomerType::CyclingEnthusiast, 4}
    };
}

} // namespace BikeShopTycoon