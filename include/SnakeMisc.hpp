/*
 *  SnakeMisc Classes, Types for Snake Demo
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

#ifndef _SNAKE_MISC_HPP_
#define _SNAKE_MISC_HPP_

#include <memory>
#include <cstdint>
#include <optional>

#include "Dim.hpp"
#include "Point.hpp"

using namespace g80;

namespace snake {
    using Fields = std::unique_ptr<Dim[]>;
    using DimOp = std::optional<Dim>;

    struct Dir {
        Dim xn, yn;
    };
    
    constexpr Dir RIGHT_DIR{1, 0};
    constexpr Dir LEFT_DIR{-1, 0};
    constexpr Dir UP_DIR{0, -1};
    constexpr Dir DOWN_DIR{0, 1};

    using PointOp = std::optional<Point>;
    
    struct VectorPoint {
        Point point; 
        Dir dir;
    };
}

#endif 