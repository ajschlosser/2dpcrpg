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

#include "engine.hpp"

template <class T>
    T& Engine::TransformObject ( T &object, StringVector targets, StringVector results, StringVector requirements ) {

    return object;
}

template <class T>
    T& Engine::AutomateObject ( T &object ) {
    for (Aspect::AspectMapIterator i = object->get_aspects().begin(); i != object->get_aspects().end(); i++) {
        Aspect a;
        LoadAspectData(a, map_aspects_.at(i->first));
        for (unsigned int j = 0; j < a.vec_effects.size(); j++) {
            std::string s = a.vec_effects[j];
            Effect e = map_effects_.at(s);
            for (unsigned int k = 0; k < e.actions.size(); k++) {
                if (!e.actions.at(k).compare("transform")) {

                }
            }
        }
    }
    return object;
}

int Engine::GetRandomNumber ( int floor, int ceiling )
{
    time_t seed;
    seed = time(NULL) + oldseed_;
    oldseed_ = seed;
    srand(seed);
    int n = ceiling - floor + 1;
    int i = rand() % n;
    if(i < 0) i = -i;
    return floor + i;
}

void Engine::ParseSettings ()
{
    Json::Reader reader;
    std::ifstream objects_file("objects.json", std::ifstream::binary);
    bool parsed = reader.parse(objects_file, objects_);
    if (!parsed) {
        std::cout  << "[ERROR]: Failed to parse configuration!" << std::endl << reader.getFormatedErrorMessages() << std::endl;
    }
}

void Engine::LoadAspects ()
{
    std::cout << "- Loading aspects..." << std::endl;
    Json::Value aspects_array = objects_["aspects"]["aspect"];
    for (unsigned int i = 0; i < aspects_array.size(); i++) {
        std::cout << "  * Loading aspect '" << aspects_array[i].get("id", "error: unnamed").asString() << "'" << std::endl;
        Aspect aspect;
        aspect.id = aspects_array[i].get("id", "").asString();
        aspect.intensity = aspects_array[i].get("intensity", 0).asInt();
        Json::Value effects_array = aspects_array[i]["effects"];
        for (unsigned int i = 0; i < effects_array.size(); i++) {
            std::string e = effects_array[i].asString();
            std::cout << "    + Adding effect '" << e << "'" << std::endl;
            aspect.vec_effects.push_back(e);
        }
        map_aspects_.insert(std::pair<std::string, Aspect>(aspect.id, aspect));
    }
    std::cout << "- Done!" << std::endl;
}

void Engine::LoadEffects ()
{
    std::cout << "- Loading effects..." << std::endl;
    Json::Value effects_array = objects_["effects"]["effect"];
    for (unsigned int i = 0; i < effects_array.size(); i++) {
        Json::Value eff = effects_array[i];
        Json::Value act = eff["actions"];
        Json::Value tar = eff["targets"];
        Json::Value res = eff["results"];
        Json::Value req = eff["requirements"];
        Effect effect;
        effect.id = eff.get("id", "").asString();
        for (unsigned int j = 0; j < act.size(); j++) {
            effect.actions.push_back(act[j].asString());
        }
        for (unsigned int j = 0; j < tar.size(); j++) {
            effect.targets.push_back(tar[j].asString());
        }
        for (unsigned int j = 0; j < res.size(); j++) {
            effect.results.push_back(res[j].asString());
        }
        for (unsigned int j = 0; j < req.size(); j++) {
            effect.requirements.push_back(req[j].asString());
        }
        std::cout << "  * Loading effect '" << effect.id << "'" << std::endl;
        map_effects_.insert(std::pair<std::string, Effect>(effect.id, effect));
    }
    std::cout << "- Done!" << std::endl;
}

void Engine::LoadObjects () {
    std::cout << "- Loading objects..." << std::endl;
    Json::Value objects_array = objects_["objects"]["object"];
    for (unsigned int i = 0; i < objects_array.size(); i++) {
        struct aspect_t {
            std::string id;
            int intensity;
        };
        std::vector<aspect_t> aspects;
        std::cout << "  * Loading '" << objects_array[i].get("id", "id").asString() << "'" << std::endl;
        Json::Value aspects_array = objects_array[i]["aspect"];
        if (aspects_array.size() > 0) {
            for (unsigned int a = 0; a < aspects_array.size(); a++) {
                aspect_t paspect;
                paspect.id = aspects_array[a].get("id", "error: unnamed").asString();
                paspect.intensity  = aspects_array[a].get("intensity", 0).asInt();
                aspects.push_back(paspect);
                std::cout << "   - " << paspect.id << " (" << paspect.intensity << ")" << std::endl;
            }
        }
        WorldObject object(
            objects_array[i].get("id", "id").asString(),
            objects_array[i].get("tileset", "tileset").asString(),
            objects_array[i].get("name", "name").asString(),
            objects_array[i].get("description", "description").asString()
        );
        if (aspects.size() > 0) {
            for (unsigned int i = 0; i < aspects.size(); i++) {
                if (map_aspects_.find(aspects[i].id) != map_aspects_.end()) {
                    object.give_aspect(aspects[i].id, map_aspects_.at(aspects[i].id), aspects[i].intensity);
                }
            }
        }
        map_objects_.insert(std::pair<std::string, WorldObject>(object.get_id(), object));
    }
}

void Engine::SaveGame ()
{

}

void Engine::GenerateWorld ()
{
    set_map_dimensions( texture_manager_.get_screen_width() * 1 / texture_manager_.get_tile_size(),
                        texture_manager_.get_screen_height() * 1 / texture_manager_.get_tile_size(),
                        3 );
    std::cout << " - Generating world (" << map_width_ << ", " << map_height_ << ", " << map_depth_ << ")..." << std::endl;
    std::cout << "  - Processing " << map_width_ * map_height_ * map_depth_ << " tiles..." << std::endl;
    for (int z = 0; z > -map_depth_; z--) {
        Map world_level ( map_width_, map_height_, z );
        for (int y = 0; y < map_height_; y++) {
            for (int x = 0; x < map_width_; x++) {
                Tile *tile = new Tile();
                tile->set_coordinates( x, y, z );
                if (GetRandomNumber(0, 100) < 95) LoadObjectData(tile,map_objects_.at("grass"));
                else LoadObjectData(tile, map_objects_.at("dirt"));
                world_level.SetTile(x, y, tile);
            }
        }
        vec_maps_.push_back(world_level);
    }
}

void Engine::Init ()
{
    is_running_ = true;
    is_refreshing_ = true;
    texture_manager_ = TextureManager();
    texture_manager_.Init();
    keyboard_ = Keyboard();
    mouse_ = Mouse();
    ParseSettings();
    LoadEffects();
    LoadAspects();
    LoadObjects();
    GenerateWorld();
}

void Engine::Quit ()
{
    is_running_ = false;
}

int Engine::HandleInput ()
{
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            std::string keyboard_input = keyboard_.GetInput(&event);
            if (keyboard_input.compare("quit") == 0) {
                Quit();
            }
            if (keyboard_input.compare("query") == 0) {

            }
            if (keyboard_input.compare("up") == 0) {
                MoveObject(player_, "up");
            } else if (keyboard_input.compare("down") == 0) {
                MoveObject(player_, "down");
            } else if (keyboard_input.compare("left") == 0) {
                MoveObject(player_, "left");
            } else if (keyboard_input.compare("right") == 0) {
                MoveObject(player_, "right");
            }
            return 1;
        }
    } else return 0;
    return 0;
}

template <class T>
    T& Engine::MoveObject(T &object, int x, int y)
{
    object->set_x(x);
    object->set_y(y);
}

template <class T>
    Offset Engine::MoveObject(T &object, std::string direction)
{
    int x = object->get_x();
    int y = object->get_y();
    Offset offset { x, y, x, y, false };
    if (direction.compare("up") == 0) offset.new_y--;
    else if (direction.compare("down") == 0) offset.new_y++;
    else if (direction.compare("left") == 0) offset.new_x--;
    else if (direction.compare("right") == 0) offset.new_x++;
    object->set_x(offset.new_x);
    object->set_y(offset.new_y);
    draw_queue_.push_back(offset);
    return offset;
}

template <class T>
    WorldObject::WorldObjectReferenceVector Engine::GetObjectsInRadius ( T &object, int radius = 1 )
{
    std::vector<WorldObject*> v;
    Map::MapIterator i = GetCurrentMap();
    for (int x = object->get_x()-radius; x < object->get_x()+radius*2; x++) {
        for (int y = object->get_y()-radius; y < object->get_y()+radius*2; y++) {
            if (x < 0) x = 0; if (y < 0) y = 0;
            std::cout << x << " " << y << std::endl;
            //v.push_back(i->GetUnit(x,y));
            //v.push_back(i->GetObject(x,y));
            v.push_back(i->GetTile(x,y));
        }
    }
    return v;
}

template <class T>
    void Engine::DrawObject (T object)
{
        int tile_size = texture_manager_.get_tile_size();
        Sprite sprite = texture_manager_.get_tileset(object->get_tileset()).get_sprite(object->get_id());
        SDL_Rect src = { sprite.tileset_x, sprite.tileset_y, 32, 32 };
        SDL_Rect dst = { object->get_x() * tile_size, object->get_y() * tile_size, tile_size, tile_size };
        SDL_RenderCopy(texture_manager_.get_renderer(), texture_manager_.get_tileset_texture(object->get_tileset()), &src, &dst);
}

void Engine::RenderObjects ()
{
    for (Map::MapIterator m = vec_maps_.begin(); m != vec_maps_.end(); m++) {
        for (int y = 0; y < map_height_; y++) {
            for (int x = 0; x < map_width_; x++) {
                if (m->get_z() == 0) {
                    Tile *tile = m->GetTile(x,y);
                    //AutomateObject(tile);
                    if (!is_refreshing_) {
                        for (std::vector<Offset>::iterator i = draw_queue_.begin(); i != draw_queue_.end(); i++) {
                            if (i->old_x == x && i->old_y == y) {
                                DrawObject(tile);
                                i->destroy = true;
                            }
                            else if (i->new_x == x && i->new_y == y) {
                                DrawObject(tile);
                                i->destroy = true;
                            }
                        }
                    } else {
                        DrawObject(tile);
                    }
                }
            }
        }
    }
    DrawObject(player_);
    for (std::vector<Offset>::iterator i = draw_queue_.begin(); i != draw_queue_.end(); i++) {
        if (i->destroy) {
            draw_queue_.erase(i);
            --i;
        }
    }
    if (is_refreshing_) is_refreshing_ = false;
}

void Engine::RenderEverything ()
{
    RenderObjects();
    SDL_RenderPresent(texture_manager_.get_renderer());
}
