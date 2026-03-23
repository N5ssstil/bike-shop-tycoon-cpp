#include "TimeManager.h"

namespace BikeShopTycoon {

TimeManager& TimeManager::Instance() {
    static TimeManager instance;
    return instance;
}

void TimeManager::Update(float deltaTime) {
    if (isPaused) return;
    
    dayProgress_ += deltaTime / secondsPerDay_;
    if (dayProgress_ >= 1.0f) {
        NextDay();
        newDayFlag_ = true;
    }
}

void TimeManager::NextDay() {
    currentDay_++;
    dayProgress_ = 0.0f;
}

bool TimeManager::IsNewDay() {
    if (newDayFlag_) {
        newDayFlag_ = false;
        return true;
    }
    return false;
}

} // namespace BikeShopTycoon