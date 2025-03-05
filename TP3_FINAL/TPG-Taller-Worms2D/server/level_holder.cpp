#include "level_holder.h"

#include <utility>
#include <vector>

#include "engine.h"

LevelHolder::LevelHolder(Battlefield& battlefield): battlefield_ref(battlefield) {}

void LevelHolder::add_bar(const float& x, const float& y, const float& angle, const bool& is_long) {
    bars.emplace_back(battlefield_ref, x, y, angle, is_long);
}

std::shared_ptr<LevelStateG> LevelHolder::get_level_building_state(const std::string& map_name) {
    uint8_t amount_of_bars = bars.size();
    std::vector<BarDto> bars_dto;
    bars_dto.reserve(amount_of_bars);

    for (auto& bar: bars) {
        b2Vec2 bar_pos = bar.get_bar_position();
        bars_dto.emplace_back(bar.get_bar_type(), bar_pos.x - bar.get_bar_width() / 2,
                              bar_pos.y - bar.get_bar_height() / 2,
                              360 - (bar.angle * (180 / M_PI)));
    }

    return (std::make_shared<LevelStateG>(map_name, amount_of_bars, std::move(bars_dto)));
}
