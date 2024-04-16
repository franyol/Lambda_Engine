#ifndef _LAMBDA_ENGINE_TTF_MANAGER_H_
#define _LAMBDA_ENGINE_TTF_MANAGER_H_

    #include "lambda_TextureManager.h"
    #include <map>
    #include <string>
    #include <iostream>

    /**
     * @brief Shortcut for calling LE_TextManager::Instance
     * */
    #define LE_TEXT LE_TextManager::Instance()

    /**
     * @brief Shortcut for deallocating LE_TextManager::destroyInstance
     * */
    #define QUIT_LE_TEXT
    
    typedef struct LE_Font {
        TTF_Font* ttf_font;
    } LE_Font;

    /**
     * @brief Class to manage text rendering
     *
     * LE_TextureManager must be initialized before LE_TextManager can be used
     * */
    class LE_TextManager
    {
        private:

            /**
             * @brief class constructor
             * */
            LE_TextManager () {}

            /**
             * @brief singleton object
             * */
            static LE_TextManager* the_instance;

            /**
             * @brief saves loaded fonts
             * */
            std::map<std::string, LE_Font*> fonts;
        public:

            /**
             * @brief class destructor
             *
             * Calls to LE_TextManager::clean
             * */
            ~LE_TextManager () { clean(); }

            /**
             * @brief get LE_TextManager::the_instance
             * */
            static LE_TextManager* Instance() {
                if ( the_instance == nullptr ) {
                    the_instance = new LE_TextManager ();
                }
                return the_instance;
            }

            /**
             * @brief Destroy LE_TextManager instance
             * */
            static void destroyInstance () {
                if ( the_instance != nullptr ) {
                    delete the_instance;
                    the_instance = nullptr;
                }
            }

            /**
             * @brief load a new font from file path
             *
             * @param ttfFilePath path to the .ttf file
             * @param size font size
             * @param fontId id for the font
             * */
            void loadFont ( std::string ttfFilePath, int size, std::string fontId ) {
                auto it = fonts.find( fontId );

                if ( it != fonts.end() ) {
                    std::cerr << "Error opening font: " << ttfFilePath <<
                        ", font ID: " << fontId << " is already in use. "
                        << "Deallocate it before loading a new font" << std::endl;
                    return;
                }

                TTF_Font* font = TTF_OpenFont ( ttfFilePath.c_str(), size );

                if ( font == nullptr ) {
                    std::cerr << "Error opening font: " << ttfFilePath <<
                        ", " << TTF_GetError() << std::endl;
                    return;
                }

                fonts[ fontId ] = new LE_Font { font };
            }

            /**
             * @brief resize a loaded font
             *
             * @param fontId
             * @param newSize
             * */
            void resizeFont ( std::string fontId, int newSize ) {
                auto it = fonts.find ( fontId );
                if ( it == fonts.end() ) {
                    std::cerr << "Error changing font size: "
                        << "Font id: " << fontId << " Doesn't exist" << std::endl;
                    return;
                }
                
                if ( TTF_SetFontSize ( it->second->ttf_font, newSize ) < 0 ) {
                    std::cerr << "Error changing font size: "
                        << TTF_GetError() << std::endl;
                }
            }

            /**
             * @brief changes font style
             *
             * This function is analog to SDL TTF_SetFontStyle
             * https://wiki.libsdl.org/SDL2_ttf/TTF_SetFontStyle
             *
             * @param fontId
             * @param style new style values to set, OR'd together
             *
             * Setting the style clears already-generated glyphs, if any, from the cache.
             * The font styles are a set of bit flags, OR'd together:
             *
             * TTF_STYLE_NORMAL
             *
             * TTF_STYLE_BOLD
             *
             * TTF_STYLE_ITALIC
             *
             * TTF_STYLE_UNDERLINE
             *
             * TTF_STYLE_STRIKETHROUGH
             * */
            void setFontStyle ( std::string fontId, int style ) {
                auto it = fonts.find ( fontId );
                if ( it == fonts.end() ) {
                    std::cerr << "Error changing font style: "
                        << "Font id: " << fontId << " Doesn't exist" << std::endl;
                    return;
                }
                TTF_SetFontStyle ( it->second->ttf_font, style );
            }

            /**
             * @brief Deallocates a font
             *
             * @param fontId
             * */
            void popFont ( std::string fontId ) {
                auto it = fonts.find ( fontId );
                if ( it != fonts.end() ) {
                    TTF_CloseFont ( it->second->ttf_font );
                    delete it->second;
                    fonts.erase(it);
                }
            }

            /**
             * @brief Creates a texture and tiles to render a text
             *
             * @param text text to render
             * @param windowId
             * @param textureId neW texture Id
             * @param fontId
             * @param tileId_prefix This method generates a tile for each line of
             * text generated, if you set tileId_prefix to "a_line_", and the texture
             * has 3 lines, the tiles generated will be called "a_line_0", "a_line_1",
             * and "a_line_2"
             * @param maxWidth maximum width of a line, if the text is larger than
             * the maxWidth, it is separated in several lines to fit the width
             * @param r red color value
             * @param g green color value
             * @param b blue color value
             * @param a alpha color value
             * @param lineskip return the font lineskip
             * @param lineheight return the line height
             *
             * @return the number of lines generated
             * */
            int createTexture 
                ( std::string text, Uint32 windowId, std::string textureId, 
                  std::string fontId,
                   std::string tileId_prefix, int maxWidth, Uint8 r, Uint8 g, Uint8 b,
                   Uint8 a, int* lineskip, int* lineheight );

            /**
             * @brief deallocates memory allocated from this object
             * */
            void clean () {
                for ( auto it = fonts.begin(); it != fonts.end(); it++ ) {
                    TTF_CloseFont ( it->second->ttf_font );
                    delete it->second;
                }
                fonts.clear();
            }
    };

#endif
