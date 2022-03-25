#include "FieldStateMgr.h"


snake::FieldStateMgr::FieldStateMgr(Area area) : 
    area_(area),
    div_(area_()), 
    egg_(area_()),
    state_(std::make_unique<Dim[]>(area_())),
    mapper_(std::make_unique<Dim[]>(area_())),
    type_(std::make_unique<StateType[]>(area_())) {
    for (Dim i = 0; i < area_(); ++i) {
        state_.get()[i] = i;
        mapper_.get()[i] = i;
        type_.get()[i] = FREE;
    }
}

auto snake::FieldStateMgr::get_ix(const Point &point) const -> Dim {
    return point.y * area_.w + point.x;
}

auto snake::FieldStateMgr::get_point(const Dim &ix) const -> Point {
    return {DIM(ix % (area_.w)), DIM(ix / (area_.w))};
}

auto snake::FieldStateMgr::is_point_used(const Point &point) const -> bool {
    return is_ix_used(get_ix(point));
}

auto snake::FieldStateMgr::is_ix_used(const Dim &ix) const -> bool {
    return mapper_[ix] >= div_;
}

auto snake::FieldStateMgr::NOT_FREE_POINT() const -> Point {
    return get_point(NOT_FREE_IX());
}

auto snake::FieldStateMgr::NOT_FREE_IX() const -> Dim {
    return area_();
}

auto snake::FieldStateMgr::set_point_as_used(const Point &point, const StateType &state_type) -> void {
    set_ix_as_used(get_ix(point), state_type);
}

auto snake::FieldStateMgr::set_ix_as_used(const Dim &ix, const StateType &state_type) -> void {
    assert(is_ix_free(ix));
    assert(state_type > FREE);

    --div_;

    // Visualization:
    //
    // mapper_[]
    // ix:  0   1   2   3   4   5   ...
    // val: 3  ... ... ... ... ... ...
    //      |
    //      +-----------+
    //                  |
    // state_[]         v
    // ix:  0   1   2   3   4   5   ...
    // val  ... ... ... 0   ... ... ...
    //                        | |
    //                        | | div_
    //                        v v
    //      <----- free ----->| | <--- used --->
    //                  

    auto mapper_ix_in_state = mapper_[ix];
    auto ix_free = state_[div_];
    std::swap(state_[mapper_[ix]], state_[div_]);
    mapper_[ix] = div_;
    mapper_[ix_free] = mapper_ix_in_state;
    type_[ix] = state_type;
}

auto snake::FieldStateMgr::set_point_as_free(const Point &point) -> void {
    set_ix_as_free(get_ix(point));
}

auto snake::FieldStateMgr::set_ix_as_free(Dim ix) -> void {
    assert(!is_ix_free(ix));
    auto mapper_ix_in_state = mapper_[ix];
    auto ix_used = state_[div_];
    std::swap(state_[mapper_[ix]], state_[div_]);
    mapper_[ix] = div_;
    mapper_[ix_used] = mapper_ix_in_state;
    type_[ix] = FREE;
    ++div_;
}

auto snake::FieldStateMgr::get_next_egg_point() -> const PointOp {
    if(!get_next_egg_ix().has_value())
        return {};
    return get_point(egg_);
} 

auto snake::FieldStateMgr::get_next_egg_ix() -> const DimOp {
    if (div_ == 0) 
        return {};
    egg_ = state_[rand() % div_];
    type_[egg_] = EGG;
    return egg_;
} 

auto snake::FieldStateMgr::get_state_ix(const Dim &ix) const -> Dim {
    return state_[ix];
}

auto snake::FieldStateMgr::get_state_point(const Point &point) const -> Dim {
    return get_state_ix(get_ix(point));
}

auto snake::FieldStateMgr::get_mapper_point(const Point &point) const -> Dim {
    return get_mapper_ix(get_ix(point));
}

auto snake::FieldStateMgr::get_mapper_ix(const Dim &ix) const -> Dim {
    return mapper_[ix];
}

auto snake::FieldStateMgr::is_point_free(const Point &point) const -> bool {
    return is_ix_free(get_ix(point));
}

auto snake::FieldStateMgr::is_ix_free(const Dim &ix) const -> bool {
    return mapper_[ix] < div_;
}

auto snake::FieldStateMgr::get_egg_point() const -> Point {
    return get_point(egg_);
}

auto snake::FieldStateMgr::get_egg_ix() const -> Dim {
    return egg_;
}

auto snake::FieldStateMgr::has_egg() const -> bool {
    return egg_ < area_();
}

auto snake::FieldStateMgr::height() const -> Dim {
    return area_.h;
}

auto snake::FieldStateMgr::width() const -> Dim {
    return area_.w;
}

auto snake::FieldStateMgr::get_state_type_point(const Point &point) const -> const StateType & {
    return get_state_type_ix(get_ix(point));
}

auto snake::FieldStateMgr::get_state_type_ix(const Dim &ix) const -> const StateType & {
    return type_[ix];
}

auto snake::FieldStateMgr::debug() -> void {
    std::cout << "\n#     :";
    for (int i = 0; i < area_(); ++i)
        std::cout << std::setw(4) << i;
    std::cout << "\nDivider:" << std::setw(4 * (div_ + 1) - 1) << "^";
    std::cout << "\nF & U  :";
    for (int i = 0; i < area_(); ++i)
        std::cout << std::setw(4) << state_[i];
    std::cout << "\nState :";
    for (int i = 0; i < area_(); ++i)
        std::cout << std::setw(4) << mapper_[i];
    std::cout << "\n\n";
}

auto snake::FieldStateMgr::show() -> void {
    for (Dim i = 0; i < area_.h; ++i) {
        for (Dim j = 0; j < area_.w; ++j) {
            std::cout << (is_point_free({j, i}) ? ' ' : '#');
        }
        std::cout << "\n";
    }
}

auto snake::FieldStateMgr::set_point_trail(const Point &point) -> void {
    set_ix_trail(get_ix(point));
}

auto snake::FieldStateMgr::set_ix_trail(const Dim &ix) -> void {
    type_[ix] = TRAIL;
}

auto snake::FieldStateMgr::clear_point_trail(const Point &point) -> void {
    clear_ix_trail(get_ix(point));
}

auto snake::FieldStateMgr::clear_ix_trail(const Dim &ix) -> void {
    type_[ix] = FREE;
}

