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

#ifndef INC_INPUT_HPP
#define INC_INPUT_HPP

#include "SDL2/SDL.h"
#include <string>
#include <map>
#include <sstream>
#include <stdexcept>

typedef std::map<std::string, void(*)> KeyboardFunctionMap;
typedef std::map<std::string, int> KeyboardMap;
typedef std::map<std::string, int>::iterator KeyBoardMapIterator;
typedef std::pair<std::string, int> KeyboardPair;
typedef std::map<std::string, int> MouseMap;
typedef std::map<std::string, int>::iterator MouseMapIterator;
typedef std::pair<std::string, int> MousePair;

class Keyboard
{
    private:
        std::string last_key_press_;
        KeyboardMap map_keyboard_;
        KeyboardFunctionMap map_keyboard_functions_;
    public:
        std::string get_last_key_press () {
            return last_key_press_;
        }
        void set_last_key_press ( std::string key_name ) {
            last_key_press_ = key_name;
        }
        int get_key ( std::string key_name ) {
            if (map_keyboard_.find(key_name) != map_keyboard_.end()) return map_keyboard_.at(key_name);
            std::stringstream message;
            message << "Could not find key '" << key_name << std::endl;
            throw std::invalid_argument(message.str());
        }
        void change_key ( std::string key_name, int new_sdl_key ) {
            if (map_keyboard_.find(key_name) != map_keyboard_.end()) map_keyboard_.at(key_name) = new_sdl_key;
            std::stringstream message;
            message << "Could not find key '" << key_name << std::endl;
            throw std::invalid_argument(message.str());
        }
        void insert_key ( std::string key_name, int sdl_key ) {
            map_keyboard_.insert(KeyboardPair(key_name, sdl_key));
        }
        std::string GetInput ( SDL_Event* );
        Keyboard ();
};

class Mouse
{
    private:
        MouseMap map_mouse_;
        int x_;
        int y_;
    public:
        int get_button ( std::string button_name ) {
            if (map_mouse_.find(button_name) != map_mouse_.end()) return map_mouse_.at(button_name);
            std::stringstream message;
            message << "Could not find button '" << button_name << std::endl;
            throw std::invalid_argument(message.str());
        }
        void change_button ( std::string button_name, int new_sdl_key ) {
            if (map_mouse_.find(button_name) != map_mouse_.end()) map_mouse_.at(button_name) = new_sdl_key;
            std::stringstream message;
            message << "Could not find button '" << button_name << std::endl;
            throw std::invalid_argument(message.str());
        }
        void insert_button ( std::string button_name, int sdl_key ) {
            map_mouse_.insert(MousePair(button_name, sdl_key));
        }
        int get_x () {
            return x_;
        }
        void set_x (int x) {
            x_ = x;
        }
        int get_y () {
            return y_;
        }
        void set_y (int y) {
            y_ = y;
        }
        std::string GetInput ( SDL_Event* );
        Mouse ();
};

#endif
