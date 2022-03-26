#include "GameFunctions.hpp"

using namespace g80;
using namespace snake;

auto main(int argc, char **argv) -> int {

    std::cout << "\n\n";
    std::cout << "****************************************************\n";
    std::cout << "* //////////////////////////////////////////////// *\n";
    std::cout << "*                                                  *\n";
    std::cout << "* A non-ECS Snake Demo of efficient Egg Generation *\n";
    std::cout << "*                      by Everett Gaius S. Vergara *\n";
    std::cout << "*                                                  *\n";
    std::cout << "* Check the full source code and exaplanation at:  *\n";
    std::cout << "* github.com/everettvergara/SnakeEggGeneration     *\n";
    std::cout << "*                                                  *\n";
    std::cout << "* %Eff of speed of new technique is 100% since     *\n";
    std::cout << "* it's O(1) efficient.                             *\n";
    std::cout << "*                                                  *\n";
    std::cout << "* %Eff of old technique to generate eggs           *\n";
    std::cout << "* with respect to new technique is:                *\n";
    std::cout << "*   unused cells / total cells x 100%              *\n";
    std::cout << "*                                                  *\n";
    std::cout << "* The efficiency deteriorates as the snake gets    *\n";
    std::cout << "* longer.                                          *\n";
    std::cout << "*                                                  *\n";
    std::cout << "****************************************************\n";
    std::cout << "\n\n";
    std::cout << " Input mode of play:\n";
    std::cout << " (1) Easy\n";
    std::cout << " (2) Hell\n\n";
    std::cout << " any other input will Exit: _";
    
    Dim mode_of_play;
    std::cin >> mode_of_play;
    if ((mode_of_play & 3) == 0)
        return 0;

    /*
     * I don't have to say this but 
     * here's the section where you declare 
     * and initialize vars.
     * 
     */

    const Area area(140, 40);
    TextImage arena(area);
    FieldStateMgr fsm(area);
    set_fsm_of_arena(fsm, (mode_of_play == 1));
    
    Snake snake({{
        DIM(area.w / 2),            // Starting X of snake
        DIM(area.h / 2)},           // Starting Y of snake
        // todo: make Direction as an alias of Point
        {1, 0}},                    // Starting Direction (X-inc, Y-inc)
        10                          // Additional length of Snake when egg is eaten
    );

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
            if(!accept_inputs(fsm, snake, path_found, is_auto))
                break;
        
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
        if (!fsm.is_point_free(snake.get_head()))
            break;

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

    } while(true);

    std::cout << "Game Over!!!" << std::endl; 
}
