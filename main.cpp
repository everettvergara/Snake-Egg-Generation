#include "GameFunctions.hpp"

using namespace g80;
using namespace snake;

auto main(int argc, char **argv) -> int {

    const Area area(140, 40);
    TextImage arena(area);
    FieldStateMgr fsm(area);
    set_fsm_of_arena(fsm);
    
    Snake snake({{
        static_cast<Dim>(area.w / 2),
        static_cast<Dim>(area.h / 2)}, {
        1, 0}}, 10);

    bool is_gameover = false;
    bool is_auto = false;
    PathFound path_found;

    do {
        TimePointSysClock start {SysClock::now()};
        
        if (is_key_pressed()) {
            char key = getchar();
            if (key == KEY_ESCAPE) {
                is_gameover = false;

            } else if (key == ' ') {
                is_auto = !is_auto;

                if (is_auto) {
                    while (!path_found.empty()) 
                        path_found.pop();

                    PathFinder path_finder(area.h, area.w); 
                    path_found = path_finder.find_path_to_egg(fsm, snake);


                }
            } else if (!is_auto) {
                if (key == 'd') snake.move_right();
                else if (key == 'a') snake.move_left();
                else if (key == 'w') snake.move_up();
                else if (key == 's') snake.move_down();
            } 
        }
        
        if (is_auto && path_found.size() > 0) {
            PathCellType path_cell_type = path_found.top();
            path_found.pop();
            if (path_cell_type == RIGHT) snake.move_right();
            else if (path_cell_type == LEFT) snake.move_left();
            else if (path_cell_type == UP) snake.move_up();
            else if (path_cell_type == DOWN) snake.move_down();
        }

        // The interested viewer should look at this function
        // get_next_egg_point();
        if (!fsm.has_egg())
            if (!fsm.get_next_egg_point().has_value())
                break;

        snake.move_head();

        if (fsm.is_point_free(snake.get_head())) {
            if (snake.does_head_hits_egg(fsm.get_egg_point())) {
                snake.increase_length();
                fsm.get_next_egg_point();

                // if (is_auto) {
                //     while (!path_found.empty()) 
                //         path_found.pop();

                //     PathFinder path_finder(area.h, area.w);
                //     path_found = path_finder.find_path_to_egg(fsm, snake);
                // }
            }
            snake.add_new_head_to_body();
            fsm.set_point_as_used(snake.get_head(), SNAKE_BODY);
            if (snake.get_body().size() > snake.get_max_body_length()) {
                fsm.set_point_as_free(snake.get_tail());
                snake.remove_tail_from_body();
            }

        } else {
            is_gameover = true;
        }

        set_arena(fsm, snake, arena, is_auto);
        arena.show();

        delay_until_next_frame(start);

    } while(!is_gameover);
}
