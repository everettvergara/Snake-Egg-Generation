/*
 *  Snake Body Class
 *  Copyright (C) 2022 Everett Gaius S. Vergara
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *	
 */


#ifndef _SNAKE_H_
#define _SNAKE_H_

// Optimize Snake Body by implementing a  
// circular list thru vector instead of deque.
// But since this is a demo of Snake Egg 
// Optimization, I'll leave the optimization to the 
// viewer.

#include <deque>

#include "Dim.hpp"
#include "FieldStateMgr.h"

namespace snake {
    
    using Body = std::deque<Point>; 
 
    class Snake {
    public:
        Snake(const VectorPoint &head, const Dim body_to_egg_ratio);
        auto move_head() -> void;
        auto get_head() -> Point &;
        auto get_tail() -> Point &;
        auto does_head_hits_egg(const Point &egg) -> bool;
        auto increase_length() -> void;
        auto increase_length_if_head_hits_egg(const Point &egg) -> void;
        auto add_new_head_to_body() -> void;
        auto remove_tail_from_body() -> void;

        auto move_up() -> void;
        auto move_down() -> void;
        auto move_left() -> void;
        auto move_right() -> void;

        auto get_body() const -> const Body &;
        auto get_max_body_length() const -> Dim;

    private:
        VectorPoint head_;
        const Dim body_to_egg_ratio_;
        Body body_;
        Dim max_body_length_; 
    };
}

#endif