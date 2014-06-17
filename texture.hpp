/*
    Copyright © 2013, 2014 Aaron John Schlosser

    This file is part of 2dpcrpg by Aaron John Schlosser.

    2dpcrpg is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    2dpcrpg is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 2dpcrpg.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INC_SURFACE_HPP
#define INC_SURFACE_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "jsoncpp/json.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>

struct Sprite
{
    std::string id;
    std::string tileset;
    int tileset_x;
    int tileset_y;
    bool animated;
    bool mobile;
    Sprite () {
        id = "Unidentified";
        tileset = "terrain";
        tileset_x = 0;
        tileset_y = 0;
        animated = false;
        mobile = false;
    }
    Sprite (std::string i, std::string t, int x, int y) {
        id = i;
        tileset = t;
        tileset_x = x;
        tileset_y = y;
        animated = false;
        mobile = false;
    }
    typedef std::map< std::string, Sprite > SpriteMap;
    typedef std::map< std::string, Sprite >::iterator SpriteMapIterator;
};

class TileSet
{
    private:
        std::string id_;
        std::string path_;
        SDL_Texture *texture_;
        Sprite::SpriteMap map_sprites_;
    public:
        typedef std::map<std::string, TileSet> TileSetMap;
        typedef std::map<std::string, TileSet>::iterator TileSetMapIterator;
        TileSet () {};
        TileSet ( std::string id, std::string path, SDL_Texture *texture ) {
            id_ = id;
            path_ = path;
            texture_ = texture;
        }
        std::string get_id () {
            return id_;
        }
        void set_id ( std::string id ) {
            id_ = id;
        }
        std::string get_path () {
            return path_;
        }
        void set_path ( std::string path ) {
            path_ = path;
        }
        SDL_Texture* get_texture () {
            return texture_;
        }
        void set_texture ( SDL_Texture *texture ) {
            texture_ = texture;
        }
        Sprite::SpriteMap get_map_sprites () {
            return map_sprites_;
        }
        Sprite get_sprite ( std::string sprite ) {
            if (map_sprites_.find(sprite) != map_sprites_.end()) return map_sprites_.at(sprite);
            std::stringstream message;
            message << "Could not find sprite '" << sprite << "' in '" << id_ << "' sprite map!" << std::endl;
            throw std::invalid_argument(message.str());
        }
        void give_sprite ( std::string id, Sprite sprite ) {
            map_sprites_.insert(std::pair<std::string, Sprite>(id, sprite));
        }
};

class FontObject
{
    private:
        std::string id_;
        const char* path_;
        int size_;
        TTF_Font* font_;
    public:
        typedef std::map<std::string, FontObject> FontMap;
        typedef std::map<std::string, FontObject>::iterator FontMapIterator;
        FontObject () {};
        FontObject ( std::string id, const char* path, int size, TTF_Font* font ) {
            id_ = id;
            path_ = path;
            size_ = size;
            font_ = font;
        }
        std::string get_id () {
            return id_;
        }
        void set_id ( std::string id ) {
            id_ = id;
        }
        const char* get_path () {
            return path_;
        }
        void set_path ( const char* path ) {
            path_ = path;
        }
        int get_size () {
            return size_;
        }
        void set_size ( int size ) {
            size_ = size;
        }
        TTF_Font* get_font () {
            return font_;
        }
        void set_font ( TTF_Font* font ) {
            font_ = font;
        }
};

class TextureManager
{
private:
    TileSet::TileSetMap map_tilesets_;
    FontObject::FontMap map_fonts_;
    int screen_bpp_;
    int screen_width_;
    int screen_height_;
    Uint32 screen_flag_;
    int font_line_spacing_;
    int tile_size_;
    Json::Value settings_;
    SDL_Window *screen_;
    SDL_Renderer *renderer_;
public:
    TextureManager () {
        font_line_spacing_ =		4;
    }
    int get_screen_bpp () {
        return screen_bpp_;
    }
    void set_screen_bpp ( int screen_bpp ) {
        screen_bpp_ = screen_bpp;
    }
    int get_screen_width () {
        return screen_width_;
    }
    void set_screen_width ( int screen_width ) {
        screen_width_ = screen_width;
    }
    int get_screen_height () {
        return screen_height_;
    }
    void set_screen_height ( int screen_height ) {
        screen_height_ = screen_height;
    }
    Uint32 get_screen_flag () {
        return screen_flag_;
    }
    void set_screen_flag ( Uint32 screen_flag ) {
        screen_flag_ = screen_flag;
    }
    int get_font_line_spacing () {
        return font_line_spacing_;
    }
    void set_font_line_spacing ( int font_line_spacing ) {
        font_line_spacing_ = font_line_spacing;
    }
    int get_tile_size () {
        return tile_size_;
    }
    SDL_Window* get_screen () {
        return screen_;
    }
    void set_screen ( SDL_Window *screen ) {
        screen_ = screen;
    }
    SDL_Renderer* get_renderer () {
        return renderer_;
    }
    void set_renderer ( SDL_Renderer *renderer ) {
        renderer_ = renderer;
    }
    SDL_Texture* get_tileset_texture ( std::string id ) {
        return map_tilesets_.at(id).get_texture();
    }
    TileSet get_tileset ( std:: string id ) {
        return map_tilesets_.at(id);
    }
    TTF_Font* get_font ( std::string id ) {
        return map_fonts_.at(id).get_font();
    }
    void ParseSettings ();
    void LoadVideoSettings ();
    void LoadTilesets ();
    void LoadSprites ();
    void LoadFonts ();
    void Init ();
    void Quit ();
};

#endif

