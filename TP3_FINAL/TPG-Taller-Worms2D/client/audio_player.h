#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <map>
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../common/const.h"


class AudioPlayer {
private:
    std::map<SoundEffects, Mix_Chunk*> effects;
    Mix_Music* background_music;
    /*
        @brief Carga todos los efectos de sonido que se van a utilizar
    */
    void load_effects();

public:
    /*
        @brief Carga la musica y todos los efectos de sonido para que esten listos para usar
    */
    AudioPlayer();
    /*
        @brief Empieza a reproducir la musica
    */
    void play_background_music();
    /*
        @param key: clave del map relacionada al efecto de audio que se quiere reproducir
        @brief Reproduce un efecto de audio
    */
    void playAudio(const SoundEffects& key);
    /*
        @brief Libera todos los efectos de audio y la musica
    */
    ~AudioPlayer();
};

#endif
