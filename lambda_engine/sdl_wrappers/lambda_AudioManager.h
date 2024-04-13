#ifndef _LAMBDA_AUDIO_MANAGER_H_
#define _LAMBDA_AUDIO_MANAGER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>
#include <cstdint>
#include <iostream>

    /**
     * @brief Shortcut for calling LE_AudioManager::the_instance
     * */
    #define LE_AUDIO LE_AudioManager::Instance()

    /**
     * @brief Shortcut for deallocating LE_AudioManager::the_instance
     * */
    #define QUIT_LE_AUDIO LE_AudioManager::destroyInstance()

    /**
     * @brief Class for managing music and sound effects
     * */
    class LE_AudioManager
    {
        private:
            /**
             * @brief Class constructor
             *
             * Initializes SDL_INIT_AUDIO if it is not initialized
             * 
             * Initializes SDL_mixer
             * */
            LE_AudioManager () {
                if ( SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO == 0 ) {
                    if ( SDL_Init ( SDL_INIT_AUDIO ) < 0 ) {
                        std::cerr << "SDL Audio could not be initialized: " 
                            << SDL_GetError() << std::endl;
                    }
                }
                if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) {
                    std::cerr << "Mixer could not initialize: " << Mix_GetError() 
                        << std::endl;
                }
            }

            /**
             * @brief store audio chunks by string ID
             * */
            std::map<std::string, Mix_Chunk*> chunks;

            /**
             * @brief store music by string ID
             * */
            std::map<std::string, Mix_Music*> tracks;

            /**
             * @brief Singleton instance
             * */
            static LE_AudioManager* the_instance;

        public:

            /**
             * @brief class destructor
             *
             * Calls to LE_AudioManager::clean
             * */
            ~LE_AudioManager () { clean(); }

            /**
             * @brief saves a Music track
             *
             * @param trackId Id to refference the track
             * @param mp3File path to file in mp3 format
             * */
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

            /**
             * @brief deallocates a Music track
             *
             * @param trackId
             * */
            void popTrack ( std::string trackId ) {
                auto it = tracks.find( trackId );
                if ( it != tracks.end() ) { 
                    Mix_FreeMusic ( it->second );
                    tracks.erase(it);
                }
            }

            /**
             * @brief Plays the music track
             *
             * @param trackId
             * @param loops number of times the track is repeated, -1 for an endless loop
             * */
            void playTrack ( std::string trackId, int loops ) {
                auto it = tracks.find( trackId );
                if ( it != tracks.end() ) { 
                    Mix_PlayMusic ( it->second, loops );
                }
            }

            /**
             * @brief load a wav chunk sound effects
             *
             * @param chunkId
             * @param wavFile path to file with wav extension
             * */
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

            /**
             * @brief deallocates Audio chunk
             *
             * @param chunkId
             * */
            void popChunk ( std::string chunkId ) {
                auto it = chunks.find( chunkId );
                if ( it != chunks.end() ) { 
                    Mix_FreeChunk ( it->second );
                    chunks.erase(it);
                }
            }

            /**
             * @brief play an Audio chunk
             *
             * @param chunkId chunk to play
             * @param loops bumber of times the chunk is repeated, -1 for endless loop
             * */
            void playChunk ( std::string chunkId, int loops ) {
                auto it = chunks.find( chunkId );
                if ( it != chunks.end() ) { 
                    Mix_PlayChannel ( -1, it->second, loops );
                }
            }

            /**
             * @brief returnns the singleton instance
             * */
            static LE_AudioManager* Instance () {
                if ( the_instance == nullptr ) {
                    the_instance = new LE_AudioManager;
                }
                return the_instance;
            }

            /**
             * @brief deallocates the singleton instance
             * */
            static void destroyInstance () {
                if ( the_instance != nullptr ) {
                    delete the_instance;
                }
            }

            /**
             * @brief deallocates audo chunks and Music tracks
             * */
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
