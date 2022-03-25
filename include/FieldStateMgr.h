/*
 *  FieldStateMgr.h - Class to handle proper Snake Egg Generation
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

#ifndef _FIELD_STATE_MANAGER_H_
#define _FIELD_STATE_MANAGER_H_

// DEBUG ON = Commented #define NDEBUG 1
// DEBUG OFF = Uncommented #define NDEBUG 1
// #define NDEBUG 1

#include <iostream>
#include <iomanip>
#include <cassert>

#include "SnakeMisc.hpp"
#include "Area.hpp"
#include "Point.hpp"
#include "FieldState.hpp"

using namespace g80;

namespace snake {

    class FieldStateMgr {
    public:
        // Constructors, Assignments 
        FieldStateMgr(Area area);
        FieldStateMgr(const FieldStateMgr &) = delete;
        FieldStateMgr(FieldStateMgr &&) = delete;
        auto operator=(const FieldStateMgr &) -> FieldStateMgr & = delete;
        auto operator=(FieldStateMgr &&) -> FieldStateMgr & = delete;
        virtual ~FieldStateMgr() = default;

        // Helper to convert Points to Ix and vice-versa
        auto get_ix(const Point &point) const -> Dim;
        auto get_point(const Dim &ix) const -> Point;

        // Check if used or free
        auto is_point_used(const Point &point) const -> bool;
        auto is_point_free(const Point &point) const -> bool;
        auto is_ix_used(const Dim &ix) const -> bool;
        auto is_ix_free(const Dim &ix) const -> bool;

        // Set as used
        auto set_point_as_used(const Point &point, const StateType &state_type) -> void;
        auto set_ix_as_used(const Dim &ix, const StateType &state_type) -> void;
        
        // Set as free
        auto set_point_as_free(const Point &point) -> void;
        auto set_ix_as_free(Dim ix) -> void;

        // Get next egg
        auto get_next_egg_point() -> const PointOp;
        auto get_next_egg_ix() -> const DimOp;

        // Get state point
        auto get_state_point(const Point &point) const -> Dim;
        auto get_state_ix(const Dim &ix) const -> Dim;
        
        // Get Mapper
        auto get_mapper_point(const Point &point) const -> Dim;
        auto get_mapper_ix(const Dim &ix) const -> Dim;

        // Get current egg position
        auto get_egg_point() const -> Point;
        auto get_egg_ix() const -> Dim;
        auto has_egg() const -> bool;

        // Dims
        auto height() const -> Dim;
        auto width() const -> Dim;
        auto get_used() const -> Dim;
        auto get_unused() const -> Dim;

        // Get State Type
        auto get_state_type_point(const Point &point) const -> const StateType &;
        auto get_state_type_ix(const Dim &ix) const -> const StateType &;
        auto set_point_trail(const Point &point) -> void;
        auto set_ix_trail(const Dim &ix) -> void;
        auto clear_point_trail(const Point &point) -> void;
        auto clear_ix_trail(const Dim &ix) -> void;

        // Debuggers
        auto debug() -> void;
        auto show() -> void;

        // INVALIDS
        auto NOT_FREE_POINT() const -> Point;
        auto NOT_FREE_IX() const -> Dim;

    protected:
        Area area_;
        Dim div_;
        Dim egg_;
        Fields state_;
        Fields mapper_;
        FieldStates type_;
    };
}

#endif