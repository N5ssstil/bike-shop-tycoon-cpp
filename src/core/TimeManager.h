#pragma once
#include <chrono>

namespace BikeShopTycoon {

class TimeManager {
public:
    static TimeManager& Instance();
    
    void Update(float deltaTime);
    void NextDay();
    
    int GetCurrentDay() const { return currentDay_; }
    float GetDayProgress() const { return dayProgress_; }
    
private:
    TimeManager() = default;
    
    int currentDay_ = 1;
    float dayProgress_ = 0.0f;
    float secondsPerDay_ = 60.0f;
};

} // namespace BikeShopTycoon