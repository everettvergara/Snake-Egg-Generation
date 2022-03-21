#include <iostream>
#include <chrono>
#include <thread>

#include "TextImage.h"
#include "Snake.hpp"
#include "FieldStateMgr.h"
#include "Misc.hpp"

using namespace g80;
using namespace snake;

constexpr int FPS = 15;
constexpr int MSPF = 1000 / FPS;
namespace chr = std::chrono;
using TimePointSysClock = chr::time_point<chr::system_clock>;
using SysClock = chr::system_clock;

auto set_fsm_of_arena(FieldStateMgr &fsm) -> void;
auto set_arena(const FieldStateMgr &fsm, const Snake &snake, TextImage &arena) -> void;
auto delay_until_next_frame(const TimePointSysClock &start) -> void;

auto main(int argc, char **argv) -> int {

    const Area area(130, 40);

    FieldStateMgr fsm(area);
    VectorPoint snake_head({{
        static_cast<Dim>(area.w / 2),
        static_cast<Dim>(area.h / 2)}, {
        1, 0
    }});
    Snake snake(snake_head, 10);
    TextImage arena(area);

    set_fsm_of_arena(fsm);
    bool is_gameover = false;
    bool is_auto = false;
    do {
        TimePointSysClock start {SysClock::now()};
        
        if (is_key_pressed()) {
            char key = getchar();
            if (key == 27) {
                is_gameover = false;
            } else if (key == ' ') {
                is_auto = !is_auto;
             } else if (!is_auto) {
                if (key == 'd') snake.move_right();
                else if (key == 'a') snake.move_left();
                else if (key == 'w') snake.move_up();
                else if (key == 's') snake.move_down();
             }
        }
        
        if (!fsm.has_egg())
            fsm.get_next_egg();

        snake.move_head();

        if (fsm.is_point_free(snake.get_head())) {
            if (snake.does_head_hits_egg(fsm.get_egg())) {
                snake.increase_length();
                fsm.get_next_egg();
            }
            snake.add_new_head_to_body();
            fsm.set_as_used(snake.get_head(), SNAKE_BODY);
            if (snake.get_body().size() > snake.get_max_body_length()) {
                fsm.set_as_free(snake.get_tail());
                snake.remove_tail_from_body();
            }
        } else {
            is_gameover = true;
        }

        set_arena(fsm, snake, arena);
        arena.show();

        delay_until_next_frame(start);

    } while(!is_gameover);

}

auto set_fsm_of_arena(FieldStateMgr &fsm) -> void {
    for (Dim i = 0; i < fsm.h(); ++i) {
        fsm.set_as_used({0, i}, BLOCK);
        fsm.set_as_used({static_cast<Dim>(fsm.w() - 1), i}, BLOCK);
    }
    for (Dim i = 1; i < fsm.w() - 1; ++i) {
        fsm.set_as_used({i, 0}, BLOCK);
        fsm.set_as_used({i, static_cast<Dim>(fsm.h() - 1)}, BLOCK);
    }
}

auto set_arena(const FieldStateMgr &fsm, const Snake &snake, TextImage &arena) -> void {
    arena.fill_text(" ");
    TextImage block("#", 7, ON);
    TextImage free(" ", 0, ON);
    TextImage egg("@", 1, ON);
    TextImage bug(".", 7, ON);

    TextImage snake_stripes[3] = {
        TextImage("$", 2, ON),
        TextImage("$", 4, ON),
        TextImage("$", 4, ON),
    };
    
    for (Dim i = 0; i < fsm.h(); ++i) {
        for (Dim j = 0; j < fsm.w(); ++j) {
            switch (fsm.get_state_type({j, i})) {
                case FREE:
                    arena.or_image(free, {j, i});
                    break;
                case BLOCK:
                    arena.or_image(block, {j, i});
                    break;
                case EGG:
                    arena.or_image(egg, {j, i});
                    break;
                default:
                    arena.or_image(bug, {j, i});
                    break;
            } 
        }
    }

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