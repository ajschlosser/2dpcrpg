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

int main ()
{
    Engine TileEngine;
    TileEngine.Init();
    WorldObject * player = new WorldObject();
    player->set_is_player(true);
    TileEngine.set_player(player);
    TileEngine.RenderEverything();
    while (TileEngine.get_is_running()) {
        TileEngine.HandleInput();
        TileEngine.RenderEverything();
    }
    return 0;
}
