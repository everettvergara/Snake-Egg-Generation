#include "GameFunctions.hpp"

using namespace g80;
using namespace snake;

auto main(int argc, char **argv) -> int {

    /*
     * I dont have to say this but 
     * here's the section where you declare 
     * and initialize vars.
     * 
     */

    const Area area(140, 40);
    TextImage arena(area);
    FieldStateMgr fsm(area);
    set_fsm_of_arena(fsm);
    
    Snake snake({{
        DIM(area.w / 2),
        DIM(area.h / 2)}, {
        1, 0}}, 10);

    bool is_gameover = false;
    bool is_auto = false;
    PathFound path_found;

    do {
        /*
         * Start calculating the elapsed 
         * Frames per second (FPS) 
         * 
         */

        TimePointSysClock start {SysClock::now()};
        
        /*
         * User input to navigate thru the
         * the game.
         * 
         * esc: quit
         * space: switch between auto and manual mode
         * w,a,s,d: move snake manually
         * 
         */

        if (is_key_pressed())
            accept_inputs(fsm, snake, path_found, is_auto, is_gameover);
        
        /*
         * If on AUTO MODE: 
         *
         * Commands the snake to move to the 
         * next closest field between the head 
         * and the egg.
         *	
         */

        if (is_auto && path_found.size() > 0) 
            auto_move_snake(snake, path_found);

        /*
         * get_next_egg_point()
         *
         * The interested reader should look at this 
         * function to see how efficiently can the 
         * generation of the next egg location be
         * vs. the traditional generation of snake egg.
         * 
         */

        if (!fsm.has_egg())
            if (!fsm.get_next_egg_point().has_value())
                break;

        /*
         * The main program routine to animate the snake,
         * perform checks and manage the field states. 
         * 
         * The class functions says it all!
         * 
         */

        snake.move_head();
        if (fsm.is_point_free(snake.get_head())) {

            if (snake.does_head_hits_egg(fsm.get_egg_point())) {
                snake.increase_length();
                fsm.get_next_egg_point();
            }

            snake.add_new_head_to_body();

            fsm.set_point_as_used(snake.get_head(), SNAKE_BODY);
            
            if (snake.get_body().size() > snake.get_max_body_length()) {
                fsm.set_point_as_free(snake.get_tail());
                snake.remove_tail_from_body();
            }

            if (is_auto && path_found.empty()) 
                get_path_found(fsm, snake, path_found);


        } else {
            is_gameover = true;
        }

        /*
         * Redraws the entire arena and show 
         * the game objects.
         * 
         */

        set_arena(fsm, snake, arena, is_auto);
        arena.show();

        /*
         * Delay until the computed Millisecs per frame 
         * to comply with the configured Frames per second.
         * 
         * Defaults to 15 FPS. Changed this at 
         * GameFunctions.hpp
         * 
         */

        delay_until_next_frame(start);

    } while(!is_gameover);
}
