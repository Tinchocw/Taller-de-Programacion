#include "audio_player.h"

#include <list>
#include <utility>

#include <spdlog/spdlog.h>

#include "../common/config.h"

#define CHUNKSIZE 4096

#define CHANNEL -1

// Max volume = MIX_MAX_VOLUME == 128
#define MUSIC_VOLUME Config::yamlNode["music_volume"].as<int>()
#define EFFECT_VOLUME Config::yamlNode["effect_volume"].as<int>()

AudioPlayer::AudioPlayer(): background_music(nullptr) {
    Mix_Init(MIX_INIT_MP3);

    if (Mix_OpenAudio(22050, AUDIO_S16SYS, MIX_DEFAULT_CHANNELS, CHUNKSIZE) == -1) {
        spdlog::get("client")->error("Error al intentar cargar el dispositivo de audio");
    }

    background_music = Mix_LoadMUS(MEDIA_PATH "/background/oriental.mp3");
    if (not background_music) {
        spdlog::get("client")->error("Error al intentar cargar la musica ambiente");
    }

    load_effects();
}

void AudioPlayer::load_effects() {
    std::list<std::pair<SoundEffects, std::string>> key_filepath;

    key_filepath.emplace_back(SoundEffects::TELEPORT, MEDIA_PATH "/TELEPORT.wav");
    key_filepath.emplace_back(SoundEffects::AIRSTRIKE, MEDIA_PATH "/AIRSTRIKE.wav");
    key_filepath.emplace_back(SoundEffects::BAZOOKA_IMPACT, MEDIA_PATH "/BAZOOKA-DRYIMPACT.wav");
    key_filepath.emplace_back(SoundEffects::EXPLOSION_WITH_FIRE, MEDIA_PATH "/FIERY-EXPLOSION.wav");
    key_filepath.emplace_back(SoundEffects::GREEN_GRENADE_IMPACT, MEDIA_PATH "/GREEN-GRENADE.wav");
    key_filepath.emplace_back(SoundEffects::RED_GRENADE_IMPACT, MEDIA_PATH "/RED-GRENADE.wav");
    key_filepath.emplace_back(SoundEffects::HOLY_GRENADE_CHOIR, MEDIA_PATH "/HOLYGRENADE.wav");
    key_filepath.emplace_back(SoundEffects::FRAGMENT_IMPACT, MEDIA_PATH "/SIZZLE.wav");
    key_filepath.emplace_back(SoundEffects::BASEBALL_IMPACT, MEDIA_PATH "/BaseBallBatImpact.wav");
    key_filepath.emplace_back(SoundEffects::BASEBALL_JINGLE, MEDIA_PATH "/BaseBallBatJingle.wav");
    key_filepath.emplace_back(SoundEffects::ROUND_START, MEDIA_PATH "/StartRound.wav");
    key_filepath.emplace_back(SoundEffects::TIMER_TICK, MEDIA_PATH "/TIMERTICK.wav");
    key_filepath.emplace_back(SoundEffects::EXTEND_WALK, MEDIA_PATH "/Walk-Expand.wav");
    key_filepath.emplace_back(SoundEffects::CONTRACT_WALK, MEDIA_PATH "/Walk-Compress.wav");
    key_filepath.emplace_back(SoundEffects::WORM_BACKFLIP, MEDIA_PATH "/BACKFLIP.wav");
    key_filepath.emplace_back(SoundEffects::WORM_JUMP, MEDIA_PATH "/JUMP.wav");
    key_filepath.emplace_back(SoundEffects::WORM_ME_MUERO, MEDIA_PATH "/OHDEAR.wav");
    key_filepath.emplace_back(SoundEffects::CRATE_LANDED, MEDIA_PATH "/CrateImpact.wav");
    key_filepath.emplace_back(SoundEffects::CRATE_OPENED, MEDIA_PATH "/cratepop.wav");

    for (const auto& key_path: key_filepath) {
        Mix_Chunk* chunk = Mix_LoadWAV(key_path.second.data());
        effects.insert({key_path.first, chunk});
        if (chunk) {
            Mix_VolumeChunk(chunk, EFFECT_VOLUME);
            continue;
        }

        spdlog::get("client")->error("Error al intentar cargar el sonido {:s}: {:s}",
                                     std::to_string(static_cast<int>(key_path.first)),
                                     key_path.second);
    }
}


void AudioPlayer::play_background_music() {
    Mix_VolumeMusic(MUSIC_VOLUME);
    if (Mix_PlayMusic(background_music, -1) == -1) {
        spdlog::get("client")->error("Error al intentar reproducir la musica ambiente");
    }
}

void AudioPlayer::playAudio(const SoundEffects& key) {
    if (effects.count(key) != 1) {
        return;
    }

    if (Mix_PlayChannel(-1, effects.at(key), 0) == -1) {
        spdlog::get("client")->error("Error al intentar reproducir el audio {:s}",
                                     std::to_string(static_cast<int>(key)));
        return;
    }
}

AudioPlayer::~AudioPlayer() {
    for (auto& [key, chunk]: effects) {
        if (chunk) {
            Mix_FreeChunk(chunk);
        }
    }

    if (background_music) {
        Mix_FreeMusic(background_music);
    }

    Mix_CloseAudio();
    Mix_Quit();
}
