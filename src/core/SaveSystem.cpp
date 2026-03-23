#include "SaveSystem.h"
#include <fstream>
#include <iostream>

namespace BikeShopTycoon {

SaveSystem& SaveSystem::Instance() {
    static SaveSystem instance;
    return instance;
}

bool SaveSystem::Save(const std::string& path) {
    // TODO: 实现存档
    return true;
}

bool SaveSystem::Load(const std::string& path) {
    // TODO: 实现读档
    return true;
}

bool SaveSystem::HasSave() const {
    std::ifstream file(savePath_);
    return file.good();
}

void SaveSystem::DeleteSave() {
    // TODO: 删除存档
}

} // namespace BikeShopTycoon