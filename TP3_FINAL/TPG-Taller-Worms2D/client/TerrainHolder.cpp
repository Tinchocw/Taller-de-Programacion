#include "TerrainHolder.h"

#include <utility>

TerrainHolder::TerrainHolder() {
    terrain_elements.insert({TerrainActors::WATER, std::list<std::unique_ptr<LevelActor>>()});
    terrain_elements.insert({TerrainActors::DEEP_WATER, std::list<std::unique_ptr<LevelActor>>()});
    terrain_elements.insert({TerrainActors::AIR_JET, std::list<std::unique_ptr<LevelActor>>()});
    terrain_elements.insert({TerrainActors::BAR, std::list<std::unique_ptr<LevelActor>>()});
    terrain_elements.insert({TerrainActors::LONG_BAR, std::list<std::unique_ptr<LevelActor>>()});
    terrain_elements.insert({TerrainActors::WIND, std::list<std::unique_ptr<LevelActor>>()});
}

void TerrainHolder::load_base_terrain(TexturesPool& txt_pool, Camera& camera) {
    terrain_elements.at(TerrainActors::WATER).emplace_back(
            std::make_unique<Water>(0, 600, txt_pool, camera));
    terrain_elements.at(TerrainActors::DEEP_WATER).emplace_back(
            std::make_unique<DeepWater>(0, 628, txt_pool, camera));
    terrain_elements.at(TerrainActors::AIR_JET).emplace_back(
            std::make_unique<AttackJet>(-50, 30, txt_pool, camera));
    terrain_elements.at(TerrainActors::WIND).emplace_back(
            std::make_unique<Wind>(980, 50, txt_pool, camera));
}

void TerrainHolder::add_terrain_element(const TerrainActors& terrain_type,
                                        std::unique_ptr<LevelActor> terrain_element) {
    terrain_elements.at(terrain_type).emplace_back(std::move(terrain_element));
}

void TerrainHolder::render_terrain(const std::shared_ptr<SDL2pp::Renderer>& game_renderer) {
    for (auto& terrain_element: terrain_elements) {
        for (auto& element: terrain_element.second) {
            element->render(game_renderer);
        }
    }
}

void TerrainHolder::update_terrain() {
    for (auto& terrain_element: terrain_elements) {
        for (auto& element: terrain_element.second) {
            element->update();
        }
    }
}

void TerrainHolder::change_jet_position(const float& x, const float& y) {
    terrain_elements.at(TerrainActors::AIR_JET).front()->change_position(x, y);
}

void TerrainHolder::update_battlefield(std::shared_ptr<BattlefieldState>& state) {
    Wind* wind_ptr = dynamic_cast<Wind*>((terrain_elements.at(TerrainActors::WIND).front()).get());
    wind_ptr->update_strenght(state->wind_force);
}
