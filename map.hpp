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

#ifndef INC_MAP_HPP
#define INC_MAP_HPP

#include <vector>
#include "object.hpp"
#include "tile.hpp"
#include "unit.hpp"

class Map
{
    int width_;
    int height_;
    int z_;
    Tile::TileVector2D vec_tiles2d_;
    Unit::UnitVector2D vec_units2d_;
    WorldObject::WorldObjectVector2D vec_objects2d_;
public:
    Map ( int width, int height, int z ) {
        width_ = width;
        height_ = height;
        z_ = z;
        //vec_things_.reserve(height*width);
        vec_tiles2d_.resize(width);
        for(int i = 0; i < width; i++) {
            vec_tiles2d_.at(i).resize(height);
        }
    }
    Map () {};
    typedef std::vector<Map> MapVector;
    typedef std::vector<Map>::iterator MapIterator;
    //Thing::ThingVector vec_things_;
    //Unit::UnitVector vec_units_;
    int get_width () {
        return width_;
    }
    void set_width ( int w ) {
        width_ = w;
    }
    int get_height () {
        return height_;
    }
    void set_height ( int h ) {
        height_ = h;
    }
    int get_z () {
        return z_;
    }
    void set_z ( int z ) {
        z_ = z;
    }
    void SetTile ( int x, int y, Tile *tile ) {
        vec_tiles2d_[x][y] = tile;
    }
    Tile* GetTile ( int x, int y ) {
        return vec_tiles2d_[x][y];
    }
    void SetUnit ( int x, int y, Unit *unit ) {
        vec_units2d_[x][y] = unit;
    }
    Unit* GetUnit ( int x, int y ) {
        return vec_units2d_[x][y];
    }
    WorldObject* GetObject ( int x, int y ) {
        return vec_objects2d_[x][y];
    }
};

#endif
