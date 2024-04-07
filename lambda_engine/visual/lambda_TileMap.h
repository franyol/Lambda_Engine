#include <vector>
#include <map>
#include <string>
#include <cstdint>

#ifndef _LAMBDA_TILE_MAP_H_
#define _LAMBDA_TILE_MAP_H_

    #define LE_TILEMAP LE_TileMapManager::Instance()
    #define QUIT_LE_TILEMAP LE_TileMapManager::destroyInstance()

    typedef uint32_t Uint32;

    class LE_TileDrawInfo
    {
        friend class LE_TileMap;

        private:
            int x;
            int y;
            double h;
            double w;
            double angle;
            bool scale;
            bool flipv;
            bool fliph;

        public:
            LE_TileDrawInfo( int m_x, int m_y, double m_h, double m_w, 
                    double m_angle, bool m_scale, bool m_flipv, bool m_fliph ):
                x(m_x), y(m_y), h(m_h), w(m_w), angle(m_angle), 
                scale(m_scale), flipv(m_flipv), fliph(m_fliph) {}
            
            ~LE_TileDrawInfo () {}
    };

    class LE_TileMap
    {
        private:
            Uint32 windowId;

            typedef std::vector<LE_TileDrawInfo*> DrawInfo_V;
            std::map<std::string, DrawInfo_V> draws;

        public:
             LE_TileMap ( Uint32 window ): windowId(window) {}
             ~LE_TileMap () {
                 for (auto it = draws.begin(); it != draws.end(); it++) {
                    DrawInfo_V infoVect = it->second;
                    for ( LE_TileDrawInfo* item : infoVect  ) {
                        delete item;
                    }
                    infoVect.clear();
                 }
                 draws.clear();
             }

             void moveMap ( int x, int y ) {
                 for ( auto it = draws.begin(); it != draws.end(); it++ ) {
                     for ( LE_TileDrawInfo* drawInfo : it->second ) {
                         drawInfo->x += x;
                         drawInfo->y += y;
                     }
                 }
             }

             void addDrawInfo ( std::string tileId, LE_TileDrawInfo* drawInfo ) {
                 auto it = draws.find( tileId );
                 if ( it == draws.end() ) {
                     draws[tileId]; // Initialize empty vector
                 }

                 draws[tileId].push_back ( drawInfo );
             }

             void drawMap ();

             void blendToTexture ( std::string textureId );
    };

    class LE_TileMapManager
    {
        private:
            LE_TileMapManager () {}

            static std::map<std::string, LE_TileMap*> projectMaps;
            static LE_TileMapManager* the_instance;

        public:
            ~LE_TileMapManager () { clean(); }
            static LE_TileMapManager* Instance () {
                if ( the_instance == nullptr ) {
                    the_instance = new LE_TileMapManager();
                }
                return the_instance;
            }
            static void destroyInstance () {
                if ( the_instance != nullptr ) {
                    delete the_instance;
                    the_instance = nullptr;
                }
            }

            void addMap ( std::string mapId, LE_TileMap* newMap ) {
                projectMaps[mapId] = newMap;
            }

            void addDrawInfo (
                    std::string mapId,
                    std::string tileId,
                    LE_TileDrawInfo* newInfo
                    ) {
                auto it = projectMaps.find(mapId);
                if (it != projectMaps.end()) {
                    it->second->addDrawInfo ( tileId, newInfo );
                }
            }

            void popMap ( std::string mapId ) {
                auto it = projectMaps.find(mapId);
                if (it != projectMaps.end()) {
                    delete it->second;
                    projectMaps.erase(it);
                }
            }

            void drawMap ( std::string mapId ) {
                auto it = projectMaps.find(mapId);
                if (it != projectMaps.end()) {
                    it->second->drawMap ();
                }
            }

            void loadFromXmlFile ( std::string filePath, Uint32 windowId );

             void blendToTexture ( std::string mapId, std::string textureId ) {
                auto it = projectMaps.find(mapId);
                if (it == projectMaps.end()) return;

                it->second->blendToTexture( textureId );
             }

            void clean() {
                for ( auto it=projectMaps.begin(); it != projectMaps.end(); it++ ) {
                    delete it->second;
                }
                projectMaps.clear();
            }
    };

#endif
