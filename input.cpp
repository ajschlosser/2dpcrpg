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

#include "input.hpp"
#include <iostream>

std::string Keyboard::GetInput ( SDL_Event *event )
{
    if (event->key.repeat && !last_key_press_.empty()) {
        std::cout << last_key_press_ << std::endl;
        return last_key_press_;
    } else {
        for (KeyBoardMapIterator i = map_keyboard_.begin(); i != map_keyboard_.end(); i++) {
            if (i->second == event->key.keysym.sym) {
                last_key_press_ = i->first;
                return i->first;
            }
        }
    }
    return "none";
}

Keyboard::Keyboard () {
    insert_key("left", SDLK_LEFT);
    insert_key("right", SDLK_RIGHT);
    insert_key("up", SDLK_UP);
    insert_key("down", SDLK_DOWN);
    insert_key("inventory", SDLK_i);
    insert_key("quit", SDLK_ESCAPE);
    insert_key("query", SDLK_q);
}

std::string Mouse::GetInput( SDL_Event *event )
{
    set_x(event->button.x);
    set_y(event->button.y);
    for (MouseMapIterator i = map_mouse_.begin(); i != map_mouse_.end(); i++) {
        if (i->second == event->button.button) {
            return i->first;
        }
    }
    return "none";
}

Mouse::Mouse() {
    insert_button("left", SDL_BUTTON_LEFT);
    insert_button("right", SDL_BUTTON_RIGHT);
    set_x(0);
    set_y(0);
}
