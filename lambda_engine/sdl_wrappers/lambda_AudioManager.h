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

    typedef struct LE_Chunk {
        Mix_Chunk* mix_chunk;
        int volume;
        int channel;
    } LE_Chunk;

    typedef struct LE_Music {
        Mix_Music* mix_music;
    } LE_Music;

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
                nReserved = 0;
                nChannels = 8; // SDL default
            }

            /**
             * @brief number of reserved channels
             * */
            int nReserved;

            /**
             * @brief number of channels
             * */
            int nChannels;

            /**
             * @brief store audio chunks by string ID
             * */
            std::map<std::string, LE_Chunk*> chunks;

            /**
             * @brief store music by string ID
             * */
            std::map<std::string, LE_Music*> tracks;

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
             * @brief get/set master volume
             *
             * @param volume new volume value (0 - 128). -1 to query
             * @return new volume value
             * */
            int masterVolume ( int volume ) { return Mix_MasterVolume( volume ); }

            /**
             * @brief get/set channel volume
             *
             * @param channel channel to modify the volume
             * @param volume new volume value (0 - 128). -1 to query
             * @return new volume value
             * */
            int channelVolume ( int channel, int volume ) { 
                return Mix_Volume ( channel, volume ); 
            }

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
                    tracks[trackId] = new LE_Music { music };
                }
            }

            /**
             * @brief get/set volume for all tracks
             *
             * @param volume new volume value (0 - 128). -1 to query
             * @return new volume value
             * */
            int trackVolume ( int volume ) { 
                return Mix_VolumeMusic ( volume );
            }

            /**
             * @brief deallocates a Music track
             *
             * @param trackId
             * */
            void popTrack ( std::string trackId ) {
                auto it = tracks.find( trackId );
                if ( it != tracks.end() ) { 
                    Mix_FreeMusic ( it->second->mix_music );
                    delete it->second;
                    tracks.erase(it);
                }
            }

            /**
             * @brief Plays the music track
             *
             * @param trackId
             * @param loops number of times the track is repeated, -1 for an endless loop
             * @param fadeIn_ms if >0, uses a fade in effect with it's duration 
             * in milli-seconds
             * */
            void playTrack ( std::string trackId, int loops, int fadeIn_ms = 0 ) {
                auto it = tracks.find( trackId );
                if ( it != tracks.end() ) { 
                    if ( fadeIn_ms > 0 )
                        Mix_FadeInMusic ( it->second->mix_music, loops, fadeIn_ms );
                    else
                        Mix_PlayMusic ( it->second->mix_music, loops );
                }
            }

            /**
             * @brief Stops playing the music channel
             *
             * All music tracks share a single channnel, so this function
             * will stop the one is playing, so no track ID nedded
             *
             * @param fadeOut_ms if >0 the music will stop with a fade-out effect with
             * it's duration in milli-seconds
             * */
            void stopTrack ( int fadeOut_ms = 0 ) {
                if ( fadeOut_ms > 0 ) {
                    Mix_FadeOutMusic ( fadeOut_ms );
                } else {
                    Mix_HaltMusic();
                }
            }

            /**
             * @brief Pause music track
             *
             * Can be resumed with LE_AudioManager::resumeTrack
             * */
            void pauseTrack () { Mix_PauseMusic(); }

            /**
             * @brief Resume paused music track
             * */
            void resumeTrack () { Mix_ResumeMusic(); }

            /**
             * @brief load a wav chunk sound effects
             *
             * @param chunkId
             * @param channel if >= 0, reserves the channels from 0 to channel, 
             * so don't reserve the channel 4 if you only need one channel reserved, 
             * since channels 0 to 3 will also be reserved,(other chunks sharing 
             * the same channel will stop the previous chunk playing) if -1, uses
             * a free chanel to play.
             * @param wavFile path to file with wav extension
             *
             * By default, SDL allocates 8 channels for chunks, if you need more
             * channels or need less than 8, use LE_AudioManager::allocateChannels
             * */
            void loadChunk ( std::string chunkId, std::string wavFile, 
                    int channel = -1 ) {
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
                    if ( channel > nChannels-1 ) {
                        nChannels = Mix_AllocateChannels ( channel + 1 );
                    }
                    if ( channel > nReserved-1 ) {
                        nReserved = Mix_ReserveChannels ( channel + 1 );
                    }
                    chunks[chunkId] = new LE_Chunk { chunk, 
                            Mix_VolumeChunk(chunk, -1), channel };
                }
            }

            /**
             * @brief get/set volume for a chunk
             *
             * returns -1 when chunk Id is not found
             *
             * @param chunkId
             * @param volume new volume value (0 - 128). -1 to query
             * @return new volume value
             * */
            int chunkVolume ( std::string chunkId, int volume ) { 
                auto it = chunks.find( chunkId );
                if ( it != chunks.end() ) { 
                    if ( volume > -1 && volume <= 128 ) 
                        it->second->volume = volume;
                    return Mix_VolumeChunk ( it->second->mix_chunk, volume );
                }
                return -1;
            }

            /**
             * @brief deallocates Audio chunk
             *
             * @param chunkId
             * */
            void popChunk ( std::string chunkId ) {
                auto it = chunks.find( chunkId );
                if ( it != chunks.end() ) { 
                    Mix_FreeChunk ( it->second->mix_chunk );
                    delete it->second;
                    chunks.erase(it);
                }
            }

            /**
             * @brief play an Audio chunk
             *
             * @param chunkId chunk to play
             * @param loops bumber of times the chunk is repeated, -1 for endless loop
             * @param fadeIn_ms if >0, uses a fade in effect with it's duration 
             * in milli-seconds
             * */
            void playChunk ( std::string chunkId, int loops, int fadeIn_ms = 0 ) {
                auto it = chunks.find( chunkId );
                if ( it != chunks.end() ) { 
                    if ( fadeIn_ms > 0 )
                        Mix_FadeInChannel ( it->second->channel,
                                it->second->mix_chunk, loops, fadeIn_ms );
                    else
                        Mix_PlayChannel ( it->second->channel, 
                                it->second->mix_chunk, loops );
                }
            }

            /**
             * @brief Stops playing the channel
             *
             * @param channel Using -1 will stop all channels
             * @param fadeOut_ms if >0 the music will stop with a fade-out effect with
             * it's duration in milli-seconds
             * */
            void stopChannel ( int channel, int fadeOut_ms = 0 ) {
                if ( fadeOut_ms > 0 ) {
                    Mix_FadeOutChannel ( channel, fadeOut_ms );
                } else {
                    Mix_HaltChannel( channel );
                }
            }

            /**
             * @brief stop current channel playback
             *
             * @param channel -1 to stop all channels
             * */
            void pauseChannel ( int channel ) { Mix_Pause( channel ); }

            /**
             * @brief resume stopped channel
             *
             * @param channel -1 to resume all channels
             * */
            void resumeChannel ( int channel ) { Mix_Resume( channel ); }

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
                    Mix_FreeChunk( it->second->mix_chunk );
                    delete it->second;
                }
                chunks.clear();
                for ( auto it = tracks.begin(); it != tracks.end(); it++ ) {
                    Mix_FreeMusic( it->second->mix_music );
                    delete it->second;
                }
                chunks.clear();
                Mix_CloseAudio();
            }
    };


#endif
