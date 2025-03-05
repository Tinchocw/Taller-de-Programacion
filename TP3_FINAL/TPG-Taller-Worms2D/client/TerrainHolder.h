#ifndef TERRAINHOLDER_H
#define TERRAINHOLDER_H

#include <list>
#include <memory>
#include <unordered_map>

#include "LevelActors.h"

class TerrainHolder {
private:
    std::unordered_map<TerrainActors, std::list<std::unique_ptr<LevelActor>>> terrain_elements;

public:
    TerrainHolder();
    void load_base_terrain(TexturesPool& txt_pool, Camera& camera);
    void add_terrain_element(const TerrainActors& terrain_type,
                             std::unique_ptr<LevelActor> terrain_element);
    void render_terrain(const std::shared_ptr<SDL2pp::Renderer>& game_renderer);
    void update_terrain();
    void change_jet_position(const float& x, const float& y);
    void update_battlefield(std::shared_ptr<BattlefieldState>& state);
    ~TerrainHolder() = default;
};

#endif  // TERRAINHOLDER_H
