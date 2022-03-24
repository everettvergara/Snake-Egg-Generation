#ifndef _PATH_FINDER_
#define _PATH_FINDER_

#include <memory>
#include "Dim.hpp"

namespace snake {

    enum PathCellType {LEFT, RIGHT, DOWN, UP, ORIGIN, DESTINATION, FREE, BLOCK};

    struct PathCell {
        Dim g, h;
        PathCellType t{FREE};

        auto f() -> Dim {
            return g + h;
        };
        
        auto projected_f(Dim projected_g) -> Dim {
            return projected_g + h;
        }
    }

    using PathCells = std::unique_ptr<PathCell[]>;
    

    class PathFinder {
    public:
        PathFinder(Dim N, Dim M) : 
            N_(N), M_(M) {};
        
    private:
        Dim N_, M_;
        PathCells path_cells;
    }
};

}

#endif 