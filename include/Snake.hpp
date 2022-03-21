#ifndef _SNAKE_HPP_
#define _SNAKE_HPP_

#include <deque>

#include "Dim.hpp"
#include "FieldStateMgr.h"

namespace snake {
    
    using Body = std::deque<Point>; 
 
 
    class Snake {
    public:
        Snake(const VectorPoint &head, const Dim body_to_egg_ratio) :
            head_(head),
            body_to_egg_ratio_(body_to_egg_ratio),
            max_body_length_(body_to_egg_ratio_) {}

        auto move_head() -> void {
            head_.point.x += head_.dir.xn;
            head_.point.y += head_.dir.yn;
        }

        auto get_head() -> Point & {
            return head_.point;
        }

        auto get_tail() -> Point & {
            return body_.back();
        }

        inline auto does_head_hits_egg(const Point &egg) -> bool {
            return head_.point == egg;
        }

        auto increase_length() -> void {
            max_body_length_ += body_to_egg_ratio_;
        }

        auto increase_length_if_head_hits_egg(const Point &egg) -> void {
            if (head_.point == egg)
                max_body_length_ += body_to_egg_ratio_;
        }

        auto add_new_head_to_body() -> void {
            // Optimize by not using a deque but by 
            // implementing a vector of circular list
            body_.push_front(head_.point);
        }

        auto remove_tail_from_body() {
            // Optimize by not using a stack but by 
            // implementing a vector of circular list
            if (body_.size() > max_body_length_)
                body_.pop_back();
        }

        auto move_up() {
            if (head_.dir.xn != 0)
                head_.dir = {0, -1};
        }

        auto move_down() {
            if (head_.dir.xn != 0)
                head_.dir = {0, 1};
        }

        auto move_left() {
            if (head_.dir.yn != 0)
                head_.dir = {-1, 0};
        }

        auto move_right() {
            if (head_.dir.yn != 0)
                head_.dir = {1, 0};
        }

        inline auto get_body() const -> const Body & {
            return body_;  
        }

        inline auto get_max_body_length() const -> Dim {
            return max_body_length_;
        }

    private:
        VectorPoint head_;
        Body body_;
        const Dim body_to_egg_ratio_;
        Dim max_body_length_; 
    };
}

#endif