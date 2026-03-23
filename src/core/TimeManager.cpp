#include "TimeManager.h"

namespace BikeShopTycoon {

TimeManager& TimeManager::Instance() {
    static TimeManager instance;
    return instance;
}

void TimeManager::Update(float deltaTime) {
    dayProgress_ += deltaTime / secondsPerDay_;
    if (dayProgress_ >= 1.0f) {
        NextDay();
    }
}

void TimeManager::NextDay() {
    currentDay_++;
    dayProgress_ = 0.0f;
}

} // namespace BikeShopTycoon