#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>

#include "GameFunctions.h"

using namespace g80;

namespace snake {

    auto remove_trail(FieldStateMgr &fsm, const Snake &snake, PathFound &path_found) -> void {
        Point trail = snake.get_head();
        while (!path_found.empty()) {
            auto t = path_found.top();

            if (t == LEFT) 
                --trail.x;
            else if (t == RIGHT) 
                ++trail.x;
            else if (t == UP) 
                --trail.y;
            else 
                ++trail.y;

            if (path_found.size() != 1)
                fsm.clear_point_trail(trail);
            path_found.pop();
        }
    }

    auto get_path_found(FieldStateMgr &fsm, const Snake &snake, PathFound &path_found) -> void {
        PathFinder path_finder(fsm.height(), fsm.width()); 
        path_found = path_finder.find_path_to_egg(fsm, snake);
    }

    auto accept_inputs(FieldStateMgr &fsm, Snake &snake, PathFound &path_found, bool &is_auto) -> bool {
        char key = getchar();
        if (key == KEY_ESCAPE) {
            return false;

        } else if (key == KEY_SPACE_BAR) {
            is_auto = !is_auto;
            remove_trail(fsm, snake, path_found);
            if (is_auto) 
                get_path_found(fsm, snake, path_found);
                
        } else if (!is_auto) {
            if (key == 'd') 
                snake.move_right();
            else if (key == 'a') 
                snake.move_left();
            else if (key == 'w') 
                snake.move_up();
            else if (key == 's') 
                snake.move_down();
        } 

        return true;
    }

    auto auto_move_snake(Snake &snake, PathFound &path_found) -> void {
        PathCellType path_cell_type = path_found.top();
        path_found.pop();
        if (path_cell_type == RIGHT) 
            snake.move_right();
        else if (path_cell_type == LEFT) 
            snake.move_left();
        else if (path_cell_type == UP) 
            snake.move_up();
        else 
            snake.move_down();
    }

    auto set_fsm_of_arena(FieldStateMgr &fsm, bool is_easy) -> void {
        for (Dim i = 0; i < fsm.height(); ++i) {
            fsm.set_point_as_used({0, i}, BLOCK);
            fsm.set_point_as_used({DIM(fsm.width() - 1), i}, BLOCK);
        }

        for (Dim i = 1; i < fsm.width() - 1; ++i) {
            fsm.set_point_as_used({i, 0}, BLOCK);
            fsm.set_point_as_used({i, DIM(fsm.height() - 1)}, BLOCK);
        }

        if (is_easy) {
            for (Dim i = 5; i < fsm.height() - 5; ++i) {
                fsm.set_point_as_used({DIM(fsm.width() / 2), i}, BLOCK);
                fsm.set_point_as_used({DIM(fsm.width() / 4), i}, BLOCK);
                fsm.set_point_as_used({DIM(fsm.width() / 2 + fsm.width() / 4), i}, BLOCK);
            }
        } else {
            std::random_device seed;
            std::default_random_engine RNG(seed());
            std::uniform_int_distribution<int32_t> rng_distribution(0, fsm.height() * fsm.width() - 1);
            for (Dim i = 0; i < fsm.height() * fsm.width() * 5 / 100; ++i) {
                Dim r = rng_distribution(RNG);
                if (fsm.get_state_type_ix(r) == FREE)
                    fsm.set_ix_as_used(r, BLOCK);
            }
        }
    }

    auto set_arena(const FieldStateMgr &fsm, const Snake &snake, TextImage &arena, bool is_auto) -> void {
        arena.fill_text(" ");
        
        float eff_value = 100.0f * fsm.get_unused() / (fsm.width() * fsm.height());
        std::stringstream ss;
        ss << " Old %Eff: " << std::fixed << std::setprecision(2) << eff_value << " vs. New %Eff: 100.00% "; 
        TextImage efficiency(ss.str(), 1, ON);
        static const TextImage automatic(" Auto (esc, ' ') ", 3, ON);
        static const TextImage manual(" Manual (esc, ' ', w,a,s,d) ", 3, ON);
        static const TextImage block("#", 7, ON);
        static const TextImage free(" ", 0, ON);
        static const TextImage egg("@", 1, ON);
        static const TextImage trail(".", 3, ON);
        static const TextImage bug("?", 7, ON);
        
        static const TextImage snake_stripes[3] = {
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
            arena.or_image(automatic, {2, 0});
        else 
            arena.or_image(manual, {2, 0});

        arena.or_image(efficiency, {30, 0});

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