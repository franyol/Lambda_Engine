#include "lambda_TextManager.h"
#include <vector>

LE_TextManager* LE_TextManager::the_instance;

int LE_TextManager::createTexture 
     ( std::string text, Uint32 windowId, std::string textureId, std::string fontId,
       std::string tileId_prefix, int maxWidth, Uint8 r, Uint8 g, Uint8 b,
       Uint8 a, int* lineskip, int* lineheight ) {

         auto it = fonts.find ( fontId );
         if ( it == fonts.end() ) {
            std::cerr << "Error creating texture from font: "
                << "Font ID: " << fontId << " Doesn't exist" << std::endl;
            return 0;
         }

         std::vector<std::string> lines;
         int count;
         int extent;
         int i;

         while ( true ) {
            if ( TTF_MeasureUTF8 ( it->second->ttf_font, text.c_str(), maxWidth, 
                    &extent, &count ) < 0 ) {
                return 0;
            }
            if ( count >= text.length() ) {
                lines.push_back ( text );
                break;
            } 
            // Get last blank space index
            for ( i = count; text[i] != ' '; i-- ) {
                if ( i == 0 )
                    break;
            }
            
            // Slice the text
            if ( i == 0 ) {
                lines.push_back( text.substr (0, count) );
                text = text.substr (i+count);
            } else {
                lines.push_back( text.substr (0, i) );
                text = text.substr (i+1);
            }
         }

         int ls, lh;
         lh = TTF_FontHeight ( it->second->ttf_font );
         ls = TTF_FontLineSkip ( it->second->ttf_font );

         if ( lineskip != nullptr ) *lineskip = ls;
         if ( lineheight != nullptr ) *lineheight = lh;

         // Create texture
         LE_TEXTURE->createTargetTexture ( windowId, textureId, 
                 lh*lines.size(), maxWidth );

         SDL_Color fg = {r, g, b, a};
         SDL_Surface* tempSurface;
         SDL_Texture* texture;
         std::string line;
         SDL_Rect dst;

         dst.x = 0;

         LE_TextureManager* tm = LE_TEXTURE;

         // Render text into that texture
         for ( int j = 0; j < lines.size(); j++ ) {
            dst.y = j*lh;
            line = lines[j];
            tempSurface = TTF_RenderUTF8_Solid(it->second->ttf_font, line.c_str(), fg);
            if ( tempSurface != nullptr ) {
                texture = SDL_CreateTextureFromSurface( 
                        tm->getRenderer(windowId),
                        tempSurface );
                if ( texture != nullptr ) {
                    // Paste texture into target
                    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
                    SDL_QueryTexture ( texture, NULL, NULL, &dst.w, &dst.h );
                    SDL_RenderCopy (
                            tm->getRenderer(windowId), texture,
                            NULL, &dst);

                    // Set a tile for this one
                    tm->createTile ( windowId, textureId, 
                            tileId_prefix + std::to_string(j), 
                            dst.x, dst.y, dst.h, dst.w );
                }
            }
         }

         tm->restoreRenderTarget ( windowId );

         return lines.size();
}

