/*
 *  FieldState.hpp - Class to handle proper Snake Egg Generation
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

#ifndef _FIELD_STATE_HPP_
#define _FIELD_STATE_HPP_

#include <memory>

using namespace g80;

namespace snake {

    enum StateType {
        FREE,
        BLOCK,
        SNAKE_BODY,
        EGG,
        TRAIL
    };

    using FieldStates = std::unique_ptr<StateType[]>;

}

#endif