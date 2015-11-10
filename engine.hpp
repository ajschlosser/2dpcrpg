/*
    Copyright © 2013, 2014, 2015 Aaron John Schlosser

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

#ifndef INC_ENGINE_HPP
#define INC_ENGINE_HPP
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "jsoncpp-new/json/json.h"
#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Button.H"
#include "FL/fl_ask.H"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include "texture.hpp"
#include "object.hpp"
#include "map.hpp"
#include "input.hpp"
#include "tile.hpp"
#include "unit.hpp"

typedef std::vector<std::string> StringVector;
typedef std::vector<std::string>::iterator StringVectorIterator;

struct Offset
{
    int old_x;
    int old_y;
    int new_x;
    int new_y;
    bool destroy;
};

class Engine
{
    private:
        int frame_rate_;
        int tile_size_;
        int map_width_;
        int map_height_;
        int map_depth_;
        int num_turns_;
        bool is_turn_based_;
        bool is_paused_;
        bool is_running_;
        bool is_refreshing_;
        long oldseed_;
        std::vector<Offset> draw_queue_;
        Json::Value objects_;
        WorldObject * player_;
        TextureManager texture_manager_;
        Map::MapVector vec_maps_;
        WorldObject::WorldObjectMap map_objects_;
        Aspect::AspectMap map_aspects_;
        Effect::EffectMap map_effects_;
        Keyboard keyboard_;
        Mouse mouse_;
    public:
            public:
                template <class T>
                    T& TransformObject (T&, StringVector, StringVector, StringVector);
                template <class T>
                    T& AutomateObject (T&);
        void set_map_dimensions ( int w, int h, int z ) {
            map_width_ = w;
            map_height_ = h;
            map_depth_ = z;
        }
        bool get_is_running () {
            return is_running_;
        }
        void set_player ( WorldObject *player ) {
            LoadObjectData(player, map_objects_.at("player"));
            player_ = player;
        }
        WorldObject* get_player () {
            return player_;
        }
        Map::MapIterator GetCurrentMap () {
            for (Map::MapIterator m = vec_maps_.begin(); m != vec_maps_.end(); m++) {
                for (int y = 0; y < map_height_; y++) {
                    for (int x = 0; x < map_width_; x++) {
                        if (m->get_z() == player_->get_z()) {
                            return m;
                        }
                    }
                }
            }
        }
        int GetRandomNumber ( int, int );
        void ParseSettings ();
        void LoadAspects ();
        void LoadEffects ();
        void LoadObjects ();
        void SaveGame ();
        void GenerateWorld ();
        void Init ();
        void Quit ();
        bool DetectCollision ();
        template <class T>
            T& MoveObject (T&, int, int);
        template <class T>
            Offset MoveObject (T&, std::string);
        template <class T>
            WorldObject::WorldObjectReferenceVector GetObjectsInRadius (T&, int);
        void DrawText ();
        template <class T>
            void DrawObject (T);
        void DisplayWindow ();
        int HandleInput ();
        void Refresh () { is_refreshing_ = true; }
        void AnimateObject ();
        void AnimateEverything ();
        void RenderObjects ();
        void RenderEverything ();
        Engine () {};
};

#endif
