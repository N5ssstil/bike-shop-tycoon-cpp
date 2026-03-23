#pragma once

#include "data/Types.h"
#include <vector>
#include <functional>

namespace BikeShopTycoon {

/**
 * 事件系统
 */
class EventSystem {
public:
    using EventCallback = std::function<void(const GameEvent&)>;
    using ChoiceCallback = std::function<void(const GameEvent&, const EventChoice&)>;
    
    EventSystem(PlayerData& playerData);
    
    // 检查并触发事件
    void CheckForEvents(int day, int reputation);
    
    // 处理选择
    void MakeChoice(GameEvent& event, const EventChoice& choice);
    
    // 获取当前事件
    const std::vector<GameEvent>& GetActiveEvents() const { return activeEvents_; }
    
    // 回调
    void SetOnEventTriggered(EventCallback cb) { onEventTriggered_ = cb; }
    void SetOnEventCompleted(EventCallback cb) { onEventCompleted_ = cb; }
    
private:
    void LoadEvents();
    void ApplyChoiceEffect(const EventChoice& choice);
    
    PlayerData& playerData_;
    std::vector<GameEvent> availableEvents_;
    std::vector<GameEvent> activeEvents_;
    
    EventCallback onEventTriggered_;
    EventCallback onEventCompleted_;
};

} // namespace BikeShopTycoon