#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>
#include <cstdint>
#include <iostream>

#ifndef _LAMBDA_AUDIO_MANAGER_H_
#define _LAMBDA_AUDIO_MANAGER_H_

    #define LE_AUDIO LE_AudioManager::Instance()
    #define QUIT_LE_AUDIO LE_AudioManager::destroyInstance()

    class LE_AudioManager
    {
        private:
            LE_AudioManager () {
                if ( SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO == 0 ) {
                    if ( SDL_Init ( SDL_INIT_AUDIO ) < 0 ) {
                        std::cerr << "SDL Audio could not be initialized: " << SDL_GetError()
                            << std::endl;
                    }
                }
                if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) {
                    std::cerr << "Mixer could not initialize: " << Mix_GetError() 
                        << std::endl;
                }
            }

            std::map<std::string, Mix_Chunk*> chunks;
            std::map<std::string, Mix_Music*> tracks;

            static LE_AudioManager* the_instance;

        public:
            ~LE_AudioManager () { clean(); }

            void loadTrack ( std::string trackId, std::string mp3File ) {
                auto it = tracks.find( trackId );
                if ( it != tracks.end() ) {
                    std::cerr << "Failed to load track " << mp3File 
                        << ": track id " << trackId << " already exists"; 
                    return;
                }
                Mix_Music *music = Mix_LoadMUS( mp3File.c_str() );
                if ( !music ) {
                    std::cerr << "Failed to load track " << mp3File << ": " 
                        << Mix_GetError() << std::endl;
                } else {
                    tracks[trackId] = music;
                }
            }
            void popTrack ( std::string trackId ) {
                auto it = tracks.find( trackId );
                if ( it != tracks.end() ) { 
                    Mix_FreeMusic ( it->second );
                    tracks.erase(it);
                }
            }
            void playTrack ( std::string trackId, int loops ) {
                auto it = tracks.find( trackId );
                if ( it != tracks.end() ) { 
                    Mix_PlayMusic ( it->second, loops );
                }
            }

            void loadChunk ( std::string chunkId, std::string wavFile ) {
                auto it = chunks.find( chunkId );
                if ( it != chunks.end() ) {
                    std::cerr << "Failed to load chunk " << wavFile 
                        << ": chunk id " << chunkId << " already exists"; 
                    return;
                }
                Mix_Chunk *chunk = Mix_LoadWAV( wavFile.c_str() );
                if ( !chunk ) {
                    std::cerr << "Failed to load chunk " << wavFile << ": " 
                        << Mix_GetError() << std::endl;
                } else {
                    chunks[chunkId] = chunk;
                }
            }
            void popChunk ( std::string chunkId ) {
                auto it = chunks.find( chunkId );
                if ( it != chunks.end() ) { 
                    Mix_FreeChunk ( it->second );
                    chunks.erase(it);
                }
            }
            void playChunk ( std::string chunkId, int loops ) {
                auto it = chunks.find( chunkId );
                if ( it != chunks.end() ) { 
                    Mix_PlayChannel ( -1, it->second, loops );
                }
            }

            static LE_AudioManager* Instance () {
                if ( the_instance == nullptr ) {
                    the_instance = new LE_AudioManager;
                }
                return the_instance;
            }

            static void destroyInstance () {
                if ( the_instance != nullptr ) {
                    delete the_instance;
                }
            }

            void clean () {
                for ( auto it = chunks.begin(); it != chunks.end(); it++ ) {
                    Mix_FreeChunk( it->second );
                }
                chunks.clear();
                for ( auto it = tracks.begin(); it != tracks.end(); it++ ) {
                    Mix_FreeMusic( it->second );
                }
                chunks.clear();
                Mix_CloseAudio();
            }
    };


#endif
