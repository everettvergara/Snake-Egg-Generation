/*
 *  GameFunctions.h - Misc Game Helpers
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

#include <chrono>
#include <thread>


#include "TextImage.h"
#include "Snake.h"
#include "FieldStateMgr.h"
#include "PathFinder.hpp"

using namespace g80;

namespace snake {
    constexpr int FPS = 15;
    constexpr int MSPF = 1000 / FPS;
    constexpr char KEY_ESCAPE = 27;
    constexpr char KEY_SPACE_BAR = 32;
    
    namespace chr = std::chrono;
    using TimePointSysClock = chr::time_point<chr::system_clock>;
    using SysClock = chr::system_clock;

    auto remove_trail(FieldStateMgr &fsm, const Snake &snake, PathFound &path_found) -> void;
    auto get_path_found(FieldStateMgr &fsm, const Snake &snake, PathFound &path_found) -> void;
    auto accept_inputs(FieldStateMgr &fsm, Snake &snake, PathFound &path_found, bool &is_auto) -> bool;
    auto auto_move_snake(Snake &snake, PathFound &path_found) -> void;
    auto set_fsm_of_arena(FieldStateMgr &fsm, bool is_easy) -> void;
    auto set_arena(const FieldStateMgr &fsm, const Snake &snake, TextImage &arena, bool is_auto) -> void;
    auto delay_until_next_frame(const TimePointSysClock &start) -> void;
}