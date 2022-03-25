#include <iostream>
#include <chrono>
#include <thread>
#include <stack>
#include <vector>
#include <tuple>

#include "TextImage.h"
#include "Snake.h"
#include "FieldStateMgr.h"
#include "Misc.hpp"
#include "PathFinder.hpp"

using namespace g80;

namespace snake {

    constexpr int FPS = 15;
    constexpr int MSPF = 1000 / FPS;
    constexpr char KEY_ESCAPE = 27;
    
    namespace chr = std::chrono;
    using TimePointSysClock = chr::time_point<chr::system_clock>;
    using SysClock = chr::system_clock;

    auto set_fsm_of_arena(FieldStateMgr &fsm) -> void {
        for (Dim i = 0; i < fsm.height(); ++i) {
            fsm.set_point_as_used({0, i}, BLOCK);
            fsm.set_point_as_used({static_cast<Dim>(fsm.width() - 1), i}, BLOCK);
        }
        for (Dim i = 1; i < fsm.width() - 1; ++i) {
            fsm.set_point_as_used({i, 0}, BLOCK);
            fsm.set_point_as_used({i, static_cast<Dim>(fsm.height() - 1)}, BLOCK);
        }

        // Add Blockades
        for (Dim i = 5; i < fsm.height() - 5; ++i) {
            fsm.set_point_as_used({static_cast<Dim>(fsm.width() / 2), i}, BLOCK);
            fsm.set_point_as_used({static_cast<Dim>(fsm.width() / 4), i}, BLOCK);
            fsm.set_point_as_used({static_cast<Dim>(fsm.width() / 2 + fsm.width() / 4), i}, BLOCK);
        }

    }

    auto set_arena(const FieldStateMgr &fsm, const Snake &snake, TextImage &arena, bool is_auto) -> void {
        arena.fill_text(" ");
        TextImage automatic("Automatic", 3, ON);
        TextImage manual("Manual", 3, ON);
        TextImage block("#", 7, ON);
        TextImage free(" ", 0, ON);
        TextImage egg("@", 1, ON);
        TextImage trail(".", 3, ON);
        TextImage bug("?", 7, ON);
        
        TextImage snake_stripes[3] = {
            TextImage("$", 2, ON),
            TextImage("$", 4, ON),
            TextImage("$", 4, ON),
        };
        
        for (Dim i = 0; i < fsm.height(); ++i) {
            for (Dim j = 0; j < fsm.width(); ++j) {
                switch (fsm.get_state_type_point({j, i})) {
                    case FREE:
                        arena.or_image(free, {j, i});
                        break;
                    case BLOCK:
                        arena.or_image(block, {j, i});
                        break;
                    case EGG:
                        arena.or_image(egg, {j, i});
                        break;
                    case TRAIL:
                        arena.or_image(trail, {j, i});
                        break;
                    default:
                        arena.or_image(bug, {j, i});
                        break;
                } 
            }
        }

        // Display instructions
        if (is_auto)
            arena.or_image(automatic, {10, 0});
        else 
            arena.or_image(manual, {10, 0});

        
        int c = 0;
        for (auto &b : snake.get_body()) {
            arena.or_image(snake_stripes[c], b);
            c = (c + 1) % 3;
        }
    }

    auto delay_until_next_frame(const TimePointSysClock &start) -> void {
        TimePointSysClock end = SysClock::now();
        int delay = MSPF - chr::duration_cast<chr::milliseconds>(end - start).count();
        if (delay > 0) std::this_thread::sleep_for(chr::milliseconds(delay));
    }
}