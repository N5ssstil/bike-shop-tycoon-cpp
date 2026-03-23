#include "EventSystem.h"
#include <random>

namespace BikeShopTycoon {

EventSystem::EventSystem(PlayerData& playerData)
    : playerData_(playerData)
{
    LoadEvents();
}

void EventSystem::CheckForEvents(int day, int reputation) {
    double baseChance = 0.15;
    if (reputation > 100) baseChance += 0.05;
    if (reputation > 300) baseChance += 0.05;
    
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    if (dist(rng) < baseChance) {
        // 找到符合条件的事件
        std::vector<GameEvent*> eligible;
        for (auto& event : availableEvents_) {
            if (day >= event.minDay && 
                reputation >= event.minReputation &&
                (event.maxReputation == 0 || reputation <= event.maxReputation)) {
                eligible.push_back(&event);
            }
        }
        
        if (!eligible.empty()) {
            std::uniform_int_distribution<int> eventDist(0, eligible.size() - 1);
            GameEvent& selected = *eligible[eventDist(rng)];
            activeEvents_.push_back(selected);
            
            if (onEventTriggered_) {
                onEventTriggered_(selected);
            }
        }
    }
}

void EventSystem::MakeChoice(GameEvent& event, const EventChoice& choice) {
    ApplyChoiceEffect(choice);
    
    // 移除事件
    activeEvents_.erase(
        std::remove_if(activeEvents_.begin(), activeEvents_.end(),
            [&](const GameEvent& e) { return e.id == event.id; }),
        activeEvents_.end()
    );
    
    if (onEventCompleted_) {
        onEventCompleted_(event);
    }
}

void EventSystem::ApplyChoiceEffect(const EventChoice& choice) {
    if (choice.moneyChange != 0) {
        playerData_.money += choice.moneyChange;
        if (playerData_.money < 0) playerData_.money = 0;
    }
    
    if (choice.reputationChange != 0) {
        playerData_.reputation += choice.reputationChange;
        playerData_.reputation = std::max(0, std::min(1000, playerData_.reputation));
    }
}

void EventSystem::LoadEvents() {
    GameEvent e1;
    e1.id = "event_lucky_day";
    e1.title = "好运降临";
    e1.description = "一位老顾客介绍了一位富豪朋友来买车！";
    e1.minDay = 1;
    e1.minReputation = 10;
    e1.choices = {
        {"热情接待，推荐最顶级的车型", 5000, 10, ""},
        {"稳扎稳打，推荐中端车型", 2000, 5, ""}
    };
    availableEvents_.push_back(e1);
    
    GameEvent e2;
    e2.id = "event_cycling_club";
    e2.title = "骑行俱乐部合作";
    e2.description = "本地骑行俱乐部希望与你的店铺建立长期合作关系。";
    e2.minDay = 5;
    e2.minReputation = 30;
    e2.choices = {
        {"同意合作，提供10%折扣", 0, 15, "Specialized"},
        {"婉拒，保持独立经营", 0, -5, ""}
    };
    availableEvents_.push_back(e2);
    
    GameEvent e3;
    e3.id = "event_complaint";
    e3.title = "顾客投诉";
    e3.description = "有顾客投诉售后问题，声称之前买的自行车有质量问题。";
    e3.minDay = 3;
    e3.minReputation = 10;
    e3.choices = {
        {"全额退款，赠送小礼品", -500, 5, ""},
        {"提供免费维修服务", -100, 0, ""},
        {"拒绝退款，坚持售后政策", 0, -15, ""}
    };
    availableEvents_.push_back(e3);
    
    GameEvent e4;
    e4.id = "event_influencer";
    e4.title = "网红打卡";
    e4.description = "一位拥有百万粉丝的骑行网红来到店里！";
    e4.minDay = 10;
    e4.minReputation = 50;
    e4.choices = {
        {"全力配合，提供最好的展示", 2000, 25, ""},
        {"正常服务，不打扰拍摄", 0, 10, ""}
    };
    availableEvents_.push_back(e4);
}

} // namespace BikeShopTycoon