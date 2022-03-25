#ifndef _PATH_FINDER_
#define _PATH_FINDER_

#include <memory>
#include <stack>
#include <unordered_set>
#include <set>
#include <cassert> 

#include "Dim.hpp"
#include "Point.hpp"
#include "FieldStateMgr.h"

namespace snake {

    enum PathCellType {LEFT, RIGHT, DOWN, UP, ORIGIN, BLOCKED_CELL, FREE_CELL, DESTINATION };

    struct PathCell {
        Dim g, h;
        PathCellType path_cell_type{FREE_CELL};

        auto f() -> Dim {
            return g + h;
        }
        
        auto projected_f(Dim projected_g) -> Dim {
            return projected_g + h;
        }
    };

    using PathCells = std::unique_ptr<PathCell[]>;
    using CellChar = char;
    using DebugCells = std::unique_ptr<CellChar[]>;
    using PathFound = std::stack<PathCellType>;
    using Consolidated_F_IX = uint32_t;
    using ToBeVisited = std::set<Consolidated_F_IX>;
    using Visited = std::unordered_set<Dim>;

    class PathFinder {
    public:
        PathFinder(Dim N, Dim M) : 
            N_(N), M_(M), S_(N_ * M_), 
            path_cells_(std::make_unique<PathCell[]>(S_)),
            debug_cells_(std::make_unique<CellChar[]>(S_)) {};

        auto display_ch() -> void {
            std::cout << "\n\nPath:\n";
            for (Dim i = 0; i < N_; ++i) {
                for (Dim j = 0; j < M_; ++j)
                    std::cout << debug_cells_[get_ix({j, i})];
                std::cout << "\n";
            }
        }

        auto display_to_be_visited() -> void {
            std::cout << "\n\nTo be Visisted:\n";
            for (auto &tbv : to_be_visited_)
                std::cout << tbv << ", ix: " << tbv % S_ << "["<< ((tbv % S_) / M_)<< "]["<< ((tbv % S_) % M_)<< "]" << " f: " << tbv / S_ << "\n";  
        }

        auto find_path_to_egg(FieldStateMgr &fsm, const Snake &snake) -> PathFound {
            
            clear();

            PathFound path_found;
            PointOp egg_location_op = copy_state_and_get_egg(fsm);
            if (!egg_location_op.has_value())
                return path_found;


            Dim c_ix = ~0, e_ix = egg_location_op.value().y * M_ + egg_location_op.value().x;
            
            compute_f_neighbor(snake.get_head(), egg_location_op.value());

            int ctr = 0;
            while(to_be_visited_.size() > 0) {
                auto top_set = to_be_visited_.begin();
                c_ix = *top_set % S_;
                Point c{static_cast<Dim>(c_ix % M_), static_cast<Dim>(c_ix / M_)};
                
                // display_to_be_visited();
                to_be_visited_.erase(top_set);
                visited_.insert(c_ix);

                if (c_ix == e_ix)
                    break;
                
                compute_f_neighbor(c, egg_location_op.value());
                // display_to_be_visited();
                // display_ch();
                // exit(0);

                ctr++;
            }

            if (c_ix == e_ix) {

                
                // std::cout << "FOUND";;
                // exit(0);

                Point stack_trace = egg_location_op.value();
                Dim st = get_ix(stack_trace);
                path_found.push(path_cells_[st].path_cell_type);
                Dim s_ix = get_ix(snake.get_head());
                    
                while(st != s_ix) {
                    
                    if (path_cells_[st].path_cell_type == RIGHT) 
                        --stack_trace.x;
                    else if (path_cells_[st].path_cell_type == LEFT)
                        ++stack_trace.x;
                    else if (path_cells_[st].path_cell_type == UP)
                        ++stack_trace.y;
                    else 
                        --stack_trace.y;

                    st = get_ix(stack_trace);
                    if (fsm.get_state_type_ix(st) == FREE)
                        fsm.set_ix_trail(st);
                    
                    if (path_cells_[st].path_cell_type == LEFT || path_cells_[st].path_cell_type == RIGHT ||
                        path_cells_[st].path_cell_type == UP || path_cells_[st].path_cell_type == DOWN)
                        path_found.push(path_cells_[st].path_cell_type);
                }

                // display_path_found(path_found);
                // display_ch();
                // exit(0);
            }

            // std::cout << "here: " << c_ix << " - " << e_ix << " ctr: " << ctr;
            // display_ch();
            // exit(0);

            return path_found;
        }

    private:
        Dim N_, M_, S_;
        PathCells path_cells_;
        DebugCells debug_cells_;
        ToBeVisited to_be_visited_;
        Visited visited_;

        auto clear() -> void {
            for (Dim i = 0; i < S_; ++i) {
                path_cells_[i].g = ~0;
                path_cells_[i].h = ~0;
                path_cells_[i].path_cell_type = FREE_CELL;
                debug_cells_[i] = ' ';
            }
        } 

        auto get_ix(const Point &p) const -> Dim {
            return p.y * M_ + p.x;
        }

        static auto get_distance(const Point &p1, const Point &p2) -> Dim {
            Dim dx = p2.x >= p1.x ? p2.x - p1.x : p1.x - p2.x;
            Dim dy = p2.y >= p1.y ? p2.y - p1.y : p1.y - p2.y;

            if (dx >= dy)
                return 14 * dy + 10 * (dx - dy);
            
            return 14 * dx + 10 * (dy - dx);
        }

        auto get_path_cell_type_name(const PathCellType &pct) -> const std::string & {
            static const std::string path_cell_type_names[] = {"<", ">", "v", "^", "o", "#", " ", "x"};
            return path_cell_type_names[pct];
        }

        auto display_path_found(PathFound path_found) -> void {
            std::cout << "\nDisplay path:\n";
            while(!path_found.empty()) {
                PathCellType cell_type = path_found.top();
                path_found.pop();
                std::cout << get_path_cell_type_name(cell_type) << "\n";
            }
        }

        auto compute_f_neighbor(const Point &p, const Point &target) -> void {

            auto calculate = [&](const Point &np, PathCellType path_cell_type) -> void {
                Dim np_ix = get_ix(np);
                Dim p_ix = get_ix(p);
                if (path_cells_[np_ix].path_cell_type >= FREE_CELL) {
                    Dim h = get_distance(np, target);
                    path_cells_[np_ix].g = 10 + path_cells_[p_ix].g;
                    path_cells_[np_ix].path_cell_type = path_cell_type;
                    path_cells_[np_ix].h = h;
                    to_be_visited_.insert(path_cells_[np_ix].f() * S_ + np_ix);
                    debug_cells_[np_ix] = '.';
                } else if (path_cells_[np_ix].projected_f(path_cells_[p_ix].g + 10) < path_cells_[np_ix].f() && 
                            visited_.find(np_ix) != visited_.end()) {
                    
                    to_be_visited_.erase(path_cells_[np_ix].f() * S_ + np_ix);
                    
                    path_cells_[np_ix].g = path_cells_[p_ix].g + 10;
                    path_cells_[np_ix].path_cell_type = path_cell_type;
                    to_be_visited_.insert(path_cells_[np_ix].f() * S_ + np_ix);
                }
            };

            if (p.x - 1 >= 0) 
                calculate({static_cast<Dim>(p.x - 1), p.y}, LEFT);

            if (p.x + 1 < M_) 
                calculate({static_cast<Dim>(p.x + 1), p.y}, RIGHT);

            if (p.y - 1 >= 0) 
                calculate({p.x, static_cast<Dim>(p.y - 1)}, UP);

            if (p.y + 1 < N_) 
                calculate({p.x, static_cast<Dim>(p.y + 1)}, DOWN);
        }

        auto copy_state_and_get_egg(const FieldStateMgr &fsm) -> PointOp {
            PointOp target_egg;
            for (Dim i = 0; i < S_; ++i) {
                StateType state_type = fsm.get_state_type_ix(i);
                switch(state_type) {
                case FREE:
                case TRAIL:
                    path_cells_[i].path_cell_type = FREE_CELL;
                    debug_cells_[i] = ' ';
                    break;
                case BLOCK:
                case SNAKE_BODY:
                    path_cells_[i].path_cell_type = BLOCKED_CELL;
                    debug_cells_[i] = '#';
                    break;
                case EGG:
                    path_cells_[i].path_cell_type = FREE_CELL;
                    target_egg.emplace(Point({static_cast<Dim>(i % M_), static_cast<Dim>(i / M_)}));
                    debug_cells_[i] = '@';
                    break;
                } 
            }
            return target_egg;
        }
    };
}



#endif 