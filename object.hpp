/*
    Copyright © 2013, 2014, 2015 Aaron John Schlosser

    This file is part of 2dpcrpg by Aaron John Schlosser.

    2dpcrpg is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 2.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INC_WORLDOBJECT_HPP
#define INC_WORLDOBJECT_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>

template<class T1, class T2>
    void LoadObjectData (T1 &t1, T2 &t2)
{
    t1->set_id(t2.get_id());
    t1->set_tileset(t2.get_tileset());
    t1->set_name(t2.get_name());
    t1->set_description(t2.get_description());
    t1->set_aspects(t2.get_aspects());
    t1->set_attributes(t2.get_attributes());
    if (t1->get_tileset().compare(t2.get_tileset()) != 0) std::cout << "[ERROR]: Could not load object!" << std::endl;
}

class WorldObject;

struct Attributes
{
    int strength;
    int defense;
    int dexterity;
    int perception;
    int max_hit_points;
    int hit_points;
    int base_speed;
    int line_of_sight;
    int weight_limit;
};

struct Animatrix
{
    int frame;
    int time_since_last_animated;
};

struct Aspect
{
    std::string id;
    std::string category;
    int intensity;
    std::vector<std::string> vec_effects;
    typedef std::map<std::string, Aspect> AspectMap;
    typedef std::map<std::string, Aspect>::iterator AspectMapIterator;
    Aspect ( std::string i, std::string c ) {
        id = i;
        category = c;
        intensity = 0;
    }
    Aspect () {}
};

void LoadAspectData (Aspect&, Aspect&);

struct Effect
{
    std::string id;
    std::vector<std::string> actions;
    std::vector<std::string> targets;
    std::vector<std::string> results;
    std::vector<std::string> requirements;
    typedef std::map<std::string, Effect> EffectMap;
    typedef std::map<std::string, Effect>::iterator EffectMapIterator;
};

class WorldObject
{
    protected:
        std::string id_;
        int x_;
        int y_;
        int z_;
        int previous_x_;
        int previous_y_;
        int layer_;
        std::string tileset_;
        std::string name_;
        std::string description_;
        int time_since_last_seen_;
        bool is_passable_;
        bool is_revealed_;
        bool is_under_fog_;
        bool is_destroyed_;
        bool is_player_;
        Attributes attributes_;
        Aspect::AspectMap map_aspects_;
        Animatrix animatrix_;
        std::vector<WorldObject> vec_objects_;
    public:
        typedef std::map<std::string, WorldObject> WorldObjectMap;
        typedef std::map<std::string, WorldObject>::iterator WorldObjectMapIterator;
        typedef std::vector<WorldObject> WorldObjectVector;
        typedef std::vector<WorldObject>::iterator WorldObjectVectorIterator;
        typedef std::vector<WorldObject*> WorldObjectReferenceVector;
        typedef std::vector<WorldObject*>::iterator WorldObjectReferenceVectorIterator;
        typedef std::vector< std::vector<WorldObject*> > WorldObjectVector2D;
        typedef std::vector< std::vector<WorldObject*> >::iterator WorldObjectVector2DIterator;
        WorldObject () {
            id_ = "unidentified";
            x_ = 0;
            y_ = 0;
            z_ = 0;
            name_ = "Unnamed";
            description_ = "No description";
            is_passable_ = true;
            is_revealed_ = false;
            is_under_fog_ = true;
            is_destroyed_ = false;
            is_player_ = false;
            time_since_last_seen_ = SDL_GetTicks();
        }
        WorldObject ( std::string id, std::string tileset, std::string name, std::string description ) {
            id_ = id;
            tileset_ = tileset;
            name_ = name;
            description_ = description;
            x_ = 0;
            y_ = 0;
            z_ = 0;
        }
        WorldObject ( std::vector<std::string> string_vec ) {
            id_ = string_vec[0];
            tileset_ = string_vec[1];
            name_ = string_vec[2];
            description_ = string_vec[3];
            x_ = 0;
            y_ = 0;
            z_ = 0;
            is_revealed_ = false;
            is_under_fog_ = true;
            is_destroyed_ = false;
            is_player_ = false;
            time_since_last_seen_ = SDL_GetTicks();
        }
        std::string get_id () {
            return id_;
        }
        void set_id ( std::string id ) {
            id_ = id;
        }
        int get_x () {
            return x_;
        }
        void set_x ( int x ) {
            previous_x_ = x_;
            x_ = x;
        }
        int get_y () {
            return y_;
        }
        void set_y ( int y ) {
            previous_y_ = y_;
            y_ = y;
        }
        int get_z () {
            return z_;
        }
        void set_z ( int z ) {
            z_ = z;
        }
        void set_coordinates ( int x, int y, int z ) {
            previous_x_ = x_;
            x_ = x;
            previous_y_ = y_;
            y_ = y;
            z_ = z;
            if (z == 0) {
                set_is_revealed(true);
                set_is_under_fog(false);
            }
        }
        int get_previous_x () {
            return previous_x_;
        }
        void set_previous_x (int previous_x ) {
            previous_x_ = previous_x;
        }
        int get_previous_y () {
            return previous_y_;
        }
        void set_previous_y (int previous_y) {
            previous_y_ = previous_y;
        }
        void set_previous_coordinates ( int previous_x, int previous_y ) {
            previous_x_ = previous_x;
            previous_y_ = previous_y;
        }
        std::string get_tileset () {
            return tileset_;
        }
        void set_tileset ( std::string tileset ) {
            tileset_ = tileset;
        }
        std::string get_name () {
            return name_;
        }
        void set_name ( std::string name ) {
            name_ = name;
        }
        std::string get_description () {
            return description_;
        }
        void set_description ( std::string description ) {
            description_ = description;
        }
        int get_time_since_last_seen () {
            return (SDL_GetTicks() - time_since_last_seen_);
        }
        void reset_time_since_last_seen () {
            time_since_last_seen_ = SDL_GetTicks();
        }
        void set_is_passable ( bool is_passable ) {
            is_passable_ = is_passable;
        }
        bool get_is_passable () {
            return is_passable_;
        }
        void set_is_revealed ( bool is_revealed ) {
            is_revealed_ = is_revealed;
        }
        bool get_is_revealed () {
            return is_revealed_;
        }
        void set_is_under_fog ( bool is_under_fog ) {
            is_under_fog_ = is_under_fog;
        }
        bool get_is_under_fog () {
            return is_under_fog_;
        }
        void set_is_destroyed ( bool is_destroyed ) {
            is_destroyed_ = is_destroyed;
        }
        bool get_is_destroyed () {
            return is_destroyed_;
        }
        void set_is_player ( bool is_player ) {
            is_player_ = is_player;
        }
        bool get_is_player () {
            return is_player_;
        }
        Attributes& get_attributes () {
            return attributes_;
        }
        void set_attributes ( Attributes &attributes ) {
            attributes_ = attributes;
        }
        void give_aspect ( std::string id, Aspect aspect, int intensity ) {
            aspect.intensity = intensity;
            map_aspects_.insert(std::pair<std::string, Aspect>(id, aspect));
        }
        void set_aspects ( Aspect::AspectMap aspects ) {
            map_aspects_ = aspects;
        }
        Aspect::AspectMap get_aspects () {
            return map_aspects_;
        }
        void Move ( std::string direction ) {
            if (direction.compare("up") == 0) this->y_ = this->y_-1;
            else if (direction.compare("down") == 0) this->y_ = this->y_+1;
            else if (direction.compare("left") == 0) this->x_ = this->x_-1;
            else if (direction.compare("right") == 0) this->x_ = this->x_+1;
        }
        template<class T>
            void GiveObject (T t1) {
                this->vec_objects_.push_back(t1);
        }
        bool operator == (WorldObject&);
};

#endif
