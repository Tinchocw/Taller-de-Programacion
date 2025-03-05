#include "death_animation.h"

#include <random>

std::shared_ptr<SDL2pp::Texture>& DeathAnimation::search_random_tomstone(TexturesPool& pool) {
    // Seed the random number generator
    std::random_device rd;
    std::mt19937 rng(rd());

    // Define a distribution (e.g., integers between 1 and 100)
    std::uniform_int_distribution<int> dist(1, 100);

    // Generate a random number
    int random_number = dist(rng);

    return pool.get_tombstone_texture(static_cast<Tombstones>(random_number % 6 + 1));
}

DeathAnimation::DeathAnimation(TexturesPool& pool, const unsigned int& delay):
        worm_death_animation((pool.get_actor_texture(Actors::DYING_WORM)), 60, 0, false),
        worm_tombstone_animation((search_random_tomstone(pool)), 20, 3, true),
        explosion(pool.get_effect_texture(VisualEffects::NORMAL_EXPLOSION), 8, 3, false),
        currentFrame(0),
        delay(delay),
        counter(0) {}

void DeathAnimation::update(const bool& iddle) {
    if (iddle) {
        return;
    } else {
        worm_death_animation.update();
        worm_tombstone_animation.update((counter < 60));
        explosion.update((counter < 60));
        counter++;
    }
}

void DeathAnimation::render(SDL2pp::Renderer& renderer, const SDL2pp::Rect& dest) {
    if (counter < 62) {
        worm_death_animation.render(renderer, dest);
    } else {
        explosion.render(renderer, dest);
        worm_tombstone_animation.render(renderer, dest);
    }
}
