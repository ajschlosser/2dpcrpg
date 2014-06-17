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

#ifndef INC_UNIT_HPP
#define INC_UNIT_HPP

struct AI
{
    int direction;
    int target_coordinates[2];
    int last_turn_acted;
    WorldObject *target_object;
};

class Unit : public WorldObject
{
    private:
        AI ai_;
    public:
        typedef std::vector< std::vector< Unit* > > UnitVector2D;
        typedef std::vector< std::vector< Unit* > >::iterator UnitVector2DIterator;
        Unit () {};
        Unit operator = (Unit& unit) {
            return unit;
        }
        AI get_AI () {
            return ai_;
        }
        void set_AI ( AI ai ) {
            ai_ = ai;
        }
};

#endif
