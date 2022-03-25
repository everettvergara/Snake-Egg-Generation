#ifndef _PATH_FINDER_
#define _PATH_FINDER_

#include <memory>
#include <stack>
#include <unordered_set>
#include <map>

#include "Dim.hpp"
#include "Point.hpp"
#include "FieldStateMgr.h"

namespace snake {

    enum PathCellType {LEFT, RIGHT, DOWN, UP, ORIGIN, DESTINATION, FREE_CELL, BLOCKED_CELL};

    struct PathCell {
        Dim g, h;
        PathCellType t{FREE_CELL};

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
    using PathFound = std::stack<PathCell>;
    using ToBeVisited = std::multimap<Dim, Dim>;
    using Visited = std::unordered_set<Dim>;

    class PathFinder {
    public:
        PathFinder(Dim N, Dim M) : 
            N_(N), M_(M), S_(N_ * M_), 
            path_cells_(std::make_unique<PathCell[]>(S_)),
            debug_cells_(std::make_unique<CellChar[]>(S_)) {};
        
        auto clear() -> void {
            for (Dim i = 0; i < S_; ++i) {
                path_cells_[i].g = ~0;
                path_cells_[i].h = ~0;
                path_cells_[i].t = FREE_CELL;
                debug_cells_[i] = ' ';
            }
        } 

        auto find_path_to_egg(FieldStateMgr &fsm, const Snake &snake) -> PathFound {
            PointOp egg_location = copy_state_and_get_egg(fsm);
            PathFound path_found;
            return path_found;
        }

    private:
        Dim N_, M_, S_;
        PathCells path_cells_;
        DebugCells debug_cells_;
        ToBeVisited to_be_visited_;
        Visited visited_;

        static auto compute_f_neighbor(const Point &p, const Point &target) -> void {

        }

        auto copy_state_and_get_egg(const FieldStateMgr &fsm) -> PointOp {
            PointOp target_egg, snake_head;
            for (Dim i = 0; i < S_; ++i) {
                StateType state_type = fsm.get_state_type_ix(i);
                switch(state_type) {
                case FREE:
                case TRAIL:
                    path_cells_[i].t = FREE_CELL;
                    break;
                case BLOCK:
                case SNAKE_BODY:
                    path_cells_[i].t = BLOCKED_CELL;
                    break;
                case EGG:
                    path_cells_[i].t = FREE_CELL;
                    target_egg.emplace(Point({static_cast<Dim>(i % M_), static_cast<Dim>(i / M_)}));
                    break;
                } 
            }
            return target_egg;
        }
    };
}



#endif 