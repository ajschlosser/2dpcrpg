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

#include "texture.hpp"

void TextureManager::ParseSettings ()
{
    Json::Reader reader;
    std::ifstream settings_file("settings.json", std::ifstream::binary);
    bool parsed = reader.parse(settings_file, settings_);
    if (!parsed) {
        std::cout  << "[ERROR]: Failed to parse configuration!" << std::endl << reader.getFormatedErrorMessages() << std::endl;
    }
}

void TextureManager::LoadVideoSettings ()
{
    if (SDL_Init(SDL_INIT_EVERYTHING)) std::cout << "[ERROR]: Problem initializing SDL: " << SDL_GetError() << "." << std::endl;
    std::cout << "- Loading video settings: " << std::endl;
    Json::Value video = settings_["video"];
    screen_width_ = video.get("width", 0).asInt();
    screen_height_ = video.get("height", 0).asInt();
    screen_flag_ = SDL_WINDOW_OPENGL;
    tile_size_ = 32;
    screen_ = SDL_CreateWindow(
        video.get("title", "Configuration file error.").asCString(),
        video.get("x", 0).asInt(),
        video.get("y", 0).asInt(),
        screen_width_,
        screen_height_,
        screen_flag_
    );
    renderer_ = SDL_CreateRenderer( screen_, -1, SDL_RENDERER_ACCELERATED);
    if (screen_ != NULL && renderer_ != NULL) std::cout << "- Done!" << std::endl;
    else std::cout << "- [ERROR]: Problem loading video settings: " << SDL_GetError() << "." << std::endl;
}

void TextureManager::LoadTilesets ()
{
    std::cout << "- Loading tilesets..." << std::endl;
    Json::Value tileset_array = settings_["tilesets"]["tileset"];
    for (unsigned int i = 0; i < tileset_array.size(); i++) {
        std::string tileset_id = tileset_array[i].get("id", "error: unnamed").asString();
        std::string tileset_path = tileset_array[i].get("path", "error: no valid path").asString();
        std::cout << "  * Loading " << tileset_id << " tileset from " << tileset_path << "... ";
        SDL_Surface *temp = IMG_Load(tileset_path.c_str());
        if (temp == NULL)std::cout << "IMG_Load not working. Is SDL2_image properly installed? ";
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer_, temp);
        if (texture == NULL)std::cout << "SDL_Texture not working. Is SDL2 properly installed? ";
        TileSet tileset( tileset_id, tileset_path, texture );
        if (tileset.get_texture() == NULL) std::cout << "[ERROR]: Could not load tileset!" << std::endl;
        else std::cout << "done!" << std::endl;
        map_tilesets_.insert(std::pair<std::string, TileSet>(tileset_id, tileset));
    }
}

void TextureManager::LoadSprites () {
    std::cout << "- Loading sprites..." << std::endl;
    Json::Value sprite_array = settings_["sprites"]["sprite"];
    for (unsigned int i = 0; i < sprite_array.size(); i++) {
        Sprite sprite (
            sprite_array[i].get("id", "error: unnamed").asString(),
            sprite_array[i].get("tileset", "error: no tileset").asString(),
            sprite_array[i].get("src_x", "error: no src_x").asInt(),
            sprite_array[i].get("src_y", "error: no src_y").asInt()
        );
        if (map_tilesets_.find(sprite.tileset) != map_tilesets_.end()) {
            map_tilesets_.at(sprite.tileset).give_sprite(sprite.id, sprite);
            std::cout << "  * Adding " << sprite.id << " to " << map_tilesets_.at(sprite.tileset).get_id() << std::endl;
        }
    }
}

void TextureManager::LoadFonts ()
{
    std::cout << "- Loading fonts..." << std::endl;
    Json::Value font_array = settings_["fonts"]["font"];
    for (unsigned int i = 0; i < font_array.size(); i++) {
        TTF_Font* temp = TTF_OpenFont(
            font_array[i].get("path", "error: no path").asCString(),
            font_array[i].get("size", 12).asInt()
        );
        FontObject font(
            font_array[i].get("id", "error: unnamed").asString(),
            font_array[i].get("path", "error: no path").asCString(),
            font_array[i].get("size", 12).asInt(),
            temp
        );
        map_fonts_.insert(std::pair<std::string, FontObject>(font.get_id(), font));
        std::cout << "  * Loading " << font.get_id() << " font at size " << font.get_size() << " from " << font.get_path() << std::endl;
    }
    std::cout << "- Done!" << std::endl;
}

void TextureManager::Init ()
{
    ParseSettings();
    LoadVideoSettings();
    LoadTilesets();
    LoadSprites();
    LoadFonts();
}
