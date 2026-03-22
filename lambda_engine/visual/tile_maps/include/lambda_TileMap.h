#ifndef _LAMBDA_TILE_MAP_H_
#define _LAMBDA_TILE_MAP_H_

    #include <vector>
    #include <map>
    #include <string>
    #include <cstdint>

    /**
     * @brief Shortcut for calling LE_TileMapManager instance
     * */
    #define LE_TILEMAP LE_TileMapManager::Instance()

    /**
     * @brief Shortcut for calling LE_TileMapManager instance destructor
     * */
    #define QUIT_LE_TILEMAP LE_TileMapManager::destroyInstance()

    typedef uint32_t Uint32;

    /**
     * @brief stores tile draw data
     *
     * LE_TileDrawInfo objects stores the necessary information for
     * scaling and positioning a tile into a window
     * */
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

    /**
     * @brief store and use groups of tiles
     *
     * A LE_TileMap object stores tile's draw information
     * using the draws map, which asigns a vector of LE_DrawInfo objects
     * to each tileId stored as the map keys.
     *
     * You can blend a tile map into a new texture, which is useful if
     * you need to scale or rotate the entire tile map, otherwise, using
     * LE_TileMap objects lets you store level designs without the need
     * of loading extra visual media.
     * */
    class LE_TileMap
    {
        private:
            /**
             * @brief windowId
             *
             * A tile map only makes sense when all the tiles and
             * textures in it belong to the same window (most of the times)
             * */
            Uint32 windowId;

            /**
             * @brief vector of LE_TileDraeInfo
             * */
            typedef std::vector<LE_TileDrawInfo*> DrawInfo_V;
            std::map<std::string, DrawInfo_V> draws;

        public:
             /**
              * @brief class constructor
              *
              * @param window window ID
              * */
             LE_TileMap ( Uint32 window ): windowId(window) {}

             /**
              * @brief class destructor
              * */
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

             /**
              * @brief relative move of every tile on the map
              *
              * @param x relative horizontal displacement
              * @param y relative vertical displacement
              *
              * Absolute movement is not supported since that requires
              * an origin to be asigned to the tile map, which we don't
              * asume at any position.
              * */
             void moveMap ( int x, int y ) {
                 for ( auto it = draws.begin(); it != draws.end(); it++ ) {
                     for ( LE_TileDrawInfo* drawInfo : it->second ) {
                         drawInfo->x += x;
                         drawInfo->y += y;
                     }
                 }
             }

             /**
              * @brief Places an instance of a tile in a map
              *
              * Remember that calling addDrawInfo several times into
              * the same tileId, will create several instances to be
              * drawn in the tile map and won't replace a previous one
              *
              * @param tileId tile to be drawn
              * @param drawInfo 
              * */
             void addDrawInfo ( std::string tileId, LE_TileDrawInfo* drawInfo ) {
                 auto it = draws.find( tileId );
                 if ( it == draws.end() ) {
                     draws[tileId]; // Initialize empty vector
                 }

                 draws[tileId].push_back ( drawInfo );
             }

             /**
              * @brief Draws the tilemap into the tile map defined window
              * */
             void drawMap ();

             /**
              * @brief creates a new texture from the tilemap
              *
              * creates a new texture with the provided textureId,
              * then draws the tilemap into it. Texture dimentions
              * are autmoatically calculated to fit the exact tilemap
              * dimensions.
              *
              * @param textureId
              * */
             void blendToTexture ( std::string textureId );
    };

    /**
     * @brief Class to manage tile maps
     * */
    class LE_TileMapManager
    {
        private:
            /**
             * @brief class constructor
             * */
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

            /**
             * @brief add a new map
             *
             * @param mapId new map id
             * @param newMap
             * */
            void addMap ( std::string mapId, LE_TileMap* newMap ) {
                projectMaps[mapId] = newMap;
            }

            /**
             * @brief add draw info into a map
             *
             * @param mapId
             * @param tileId
             * @param newInfo
             * */
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

            /**
             * @brief deallocate map information
             *
             * Does not deallocate tiles or textures!
             *
             * @param mapId
             * */
            void popMap ( std::string mapId ) {
                auto it = projectMaps.find(mapId);
                if (it != projectMaps.end()) {
                    delete it->second;
                    projectMaps.erase(it);
                }
            }

            /**
             * @brief draw map into it's window
             *
             * @param mapId
             * */
            void drawMap ( std::string mapId ) {
                auto it = projectMaps.find(mapId);
                if (it != projectMaps.end()) {
                    it->second->drawMap ();
                }
            }

            /**
             * @brief load map from xml file
             *
             *
             * @param filePath
             * @param windowId
             *
             * xml files to load tilemaps must start with TILEMAPS node tag
             *
             * afterwards, each tilemap tag will append a new tilemap to
             * the manager using its id attribute as the tilemap ID.
             *
             * each tilemap has a list of set tags with a tile attribute
             * referencing the tile id to be added
             *
             * each tile id has a list of pos tags which attributes are 
             * the same used to create LE_TileDrawInfo objects
             *
             * Example: Loading "forest-background" and "sunset-background"
             * @code
             * <TILEMAPS>
             *   <tilemap id="forest-background">
             *       <set tile="sky-forest">
             *           <pos x="0" y="0" h="2" w="2" angle="0" scale="true" flipv="false" fliph="false"/>
             *       </set>
             *       <set tile="floor-forest">
             *           <pos x="0" y="96" h="2" w="2" angle="0" scale="true" flipv="false" fliph="false"/>
             *           <pos x="48" y="96" h="2" w="2" angle="0" scale="true" flipv="false" fliph="false"/>
             *           <pos x="144" y="96" h="2" w="2" angle="0" scale="true" flipv="false" fliph="false"/>
             *       </set>
             *   </tilemap>
             *   <tilemap id="sunset-background">
             *       <set tile="sky-sunset">
             *           <pos x="144" y="0" h="2" w="2" angle="0" scale="true" flipv="false" fliph="false"/>
             *       </set>
             *       <set tile="floor-sunset">
             *           <pos x="96" y="96" h="2" w="2" angle="0" scale="true" flipv="false" fliph="false"/>
             *           <pos x="144" y="96" h="2" w="2" angle="0" scale="true" flipv="false" fliph="false"/>
             *       </set>
             *   </tilemap>
             * </TILEMAPS>
             * @endcode
             * */
            void loadFromXmlFile ( std::string filePath, Uint32 windowId );

            /**
             * @brief creates a new texture from the tilemap
             *
             * creates a new texture with the provided textureId,
             * then draws the tilemap into it. Texture dimentions
             * are autmoatically calculated to fit the exact tilemap
             * dimensions.
             *
             * @param mapId
             * @param textureId new texture id
             * */
            void blendToTexture ( std::string mapId, std::string textureId ) {
                auto it = projectMaps.find(mapId);
                if (it == projectMaps.end()) return;

                it->second->blendToTexture( textureId );
            }

            /**
             * @brief deallocate all maps
             * */
            void clean() {
                for ( auto it=projectMaps.begin(); it != projectMaps.end(); it++ ) {
                    delete it->second;
                }
                projectMaps.clear();
            }
    };

#endif
