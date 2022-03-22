#include "Snake.h"

snake::Snake::Snake(const VectorPoint &head, const Dim body_to_egg_ratio) :
    head_(head),
    body_to_egg_ratio_(body_to_egg_ratio),
    max_body_length_(body_to_egg_ratio_) {
}

auto snake::Snake::move_head() -> void {
    head_.point.x += head_.dir.xn;
    head_.point.y += head_.dir.yn;
}

auto snake::Snake::get_head() -> Point & {
    return head_.point;
}

auto snake::Snake::get_tail() -> Point & {
    return body_.back();
}

auto snake::Snake::does_head_hits_egg(const Point &egg) -> bool {
    return head_.point == egg;
}

auto snake::Snake::increase_length() -> void {
    max_body_length_ += body_to_egg_ratio_;
}

auto snake::Snake::increase_length_if_head_hits_egg(const Point &egg) -> void {
    if (head_.point == egg)
        max_body_length_ += body_to_egg_ratio_;
}

auto snake::Snake::add_new_head_to_body() -> void {
    body_.push_front(head_.point);
}

auto snake::Snake::remove_tail_from_body() -> void {
    if (body_.size() > max_body_length_)
        body_.pop_back();
}

auto snake::Snake::move_up() -> void {
    if (head_.dir.xn != 0)
        head_.dir = {0, -1};
}

auto snake::Snake::move_down() -> void{
    if (head_.dir.xn != 0)
        head_.dir = {0, 1};
}

auto snake::Snake::move_left() -> void {
    if (head_.dir.yn != 0)
        head_.dir = {-1, 0};
}

auto snake::Snake::move_right() -> void {
    if (head_.dir.yn != 0)
        head_.dir = {1, 0};
}

auto snake::Snake::get_body() const -> const Body & {
    return body_;  
}

auto snake::Snake::get_max_body_length() const -> Dim {
    return max_body_length_;
}