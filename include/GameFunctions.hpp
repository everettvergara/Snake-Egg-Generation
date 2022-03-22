#include <iostream>
#include <chrono>
#include <thread>
#include <stack>

#include "TextImage.h"
#include "Snake.h"
#include "FieldStateMgr.h"
#include "Misc.hpp"

using namespace g80;

namespace snake {

    constexpr int FPS = 15;
    constexpr int MSPF = 1000 / FPS;
    constexpr char KEY_ESCAPE = 27;
    
    namespace chr = std::chrono;
    using TimePointSysClock = chr::time_point<chr::system_clock>;
    using SysClock = chr::system_clock;

    enum DIRECTION {UP, DOWN, LEFT, RIGHT, ORIGIN};
    struct Path {Point p; Dim n; DIRECTION d;};
    using PathFinder = std::stack<Path>;
    using FoundPath = std::stack<DIRECTION>;

    auto path_finder(FieldStateMgr &fsm, const Snake &snake) -> FoundPath {
        FoundPath found_path;
        PathFinder path_finder;

        if (!fsm.has_egg())
            return found_path;
        
        // Cache Arena from current FSM
        struct Arena {Dim state_type_id; DIRECTION d;};
        Arena arena[fsm.height()][fsm.width()];
        for (Dim i = 0; i < fsm.height(); ++i) {
            for (Dim j = 0; j < fsm.width(); ++j) {
                StateType state_type = fsm.get_state_type_point({i, j});
                
                if (state_type == EGG)
                    arena[i][j].state_type_id = -2;
                else if (state_type == FREE || state_type == TRAIL)
                    arena[i][j].state_type_id = -3;
                else
                    arena[i][j].state_type_id = -1;
            }
        }

        Point p = snake.get_head();
        path_finder.push({p, 0, ORIGIN});
        arena[p.y][p.x].state_type_id = 0;
        arena[p.y][p.x].d = ORIGIN;
        

        Point egg{-1, -1};
        do {
            Path path = path_finder.top();
            path_finder.pop();

            if (arena[path.p.y][path.p.x].state_type_id == -2) {
                egg = path.p;
                break;
            }

            if (path.p.x - 1 >= 0 && arena[path.p.y][path.p.x - 1].state_type_id < -1) {
                arena[path.p.y][path.p.x - 1].state_type_id = path.n + 1;
                arena[path.p.y][path.p.x - 1].d = LEFT;
                path_finder.push({{path.p.y, static_cast<Dim>(path.p.x - 1)}, static_cast<Dim>(path.n + 1)});
            }

            if (path.p.x + 1 < fsm.width() && arena[path.p.y][path.p.x + 1].state_type_id < -1) {
                arena[path.p.y][path.p.x + 1].state_type_id = path.n + 1;
                arena[path.p.y][path.p.x + 1].d = RIGHT;
                path_finder.push({{path.p.y, static_cast<Dim>(path.p.x + 1)}, static_cast<Dim>(path.n + 1)});
            }

            if (path.p.y - 1 >= 0 && arena[path.p.y - 1][path.p.x].state_type_id < -1) {
                arena[path.p.y - 1][path.p.x].state_type_id = path.n + 1;
                arena[path.p.y - 1][path.p.x].d = UP;
                path_finder.push({{static_cast<Dim>(path.p.y - 1), path.p.x}, static_cast<Dim>(path.n + 1)});
            }

            if (path.p.y + 1 < fsm.height() && arena[path.p.y + 1][path.p.x].state_type_id < -1) {
                arena[path.p.y + 1][path.p.x].state_type_id = path.n + 1;
                arena[path.p.y + 1][path.p.x].d = DOWN;
                path_finder.push({{static_cast<Dim>(path.p.y + 1), path.p.x}, static_cast<Dim>(path.n + 1)});
            }

        } while (!path_finder.empty());

        if (egg.x >= 0) {
            while (arena[egg.y][egg.x].d != ORIGIN) {
                
                found_path.push(arena[egg.y][egg.x].d);
                switch(arena[egg.y][egg.x].d) {
                    case UP:
                        ++egg.y;
                        break;
                    case DOWN:
                        --egg.y;
                        break;
                    case LEFT:
                        --egg.x;
                        break;
                    case RIGHT:
                        ++egg.x;
                        break;
                }
            }
        }

        return found_path;
    }

    auto set_fsm_of_arena(FieldStateMgr &fsm) -> void {
        for (Dim i = 0; i < fsm.height(); ++i) {
            fsm.set_point_as_used({0, i}, BLOCK);
            fsm.set_point_as_used({static_cast<Dim>(fsm.width() - 1), i}, BLOCK);
        }
        for (Dim i = 1; i < fsm.width() - 1; ++i) {
            fsm.set_point_as_used({i, 0}, BLOCK);
            fsm.set_point_as_used({i, static_cast<Dim>(fsm.height() - 1)}, BLOCK);
        }
    }

    auto set_arena(const FieldStateMgr &fsm, const Snake &snake, TextImage &arena, bool is_auto) -> void {
        arena.fill_text(" ");
        TextImage automatic(" Snake Egg Generation Demo: Automatic Play Mode... Esc to Exit, Spacebar to set to Manual Play!", 3, ON);
        TextImage manual(" Snake Egg Generation Demo: Manual Mode (Use W,A,S,D) Esc to Exit, Spacebar to set to Automatic Play! ", 3, ON);
        TextImage block("#", 7, ON);
        TextImage free(" ", 0, ON);
        TextImage egg("@", 1, ON);
        TextImage trail(".", 7, ON);
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
            arena.or_image(automatic, {2, 0});
        else 
            arena.or_image(manual, {2, 0});

        
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