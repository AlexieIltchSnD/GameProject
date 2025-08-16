#include "Track_Base.h"

Track_Base::Track_Base(const double _course, const double _speed, const Point& _position)
: course(_course), speed(_speed), position(_position) {}

Point Track_Base::get_position() const {
    return position;
}

double Track_Base::get_course() const {
    return course;
}

double Track_Base::get_speed() const {
    return speed;
}

// User for Update each vehicle for a new position.
void Track_Base::add_position(const double x, const double y) {
    position.x += x;
    position.y += y;
}

void Track_Base::set_course(const double _course) {
    course = _course;
}

void Track_Base::set_parameters(const double _speed, const double _course) {
    speed = _speed;
    course = _course;
}

void Track_Base::set_position(const Point& _position) {
    position.x = _position.x;
    position.y = _position.y;
}

void Track_Base::set_speed(const double _speed) {
    speed = _speed;
}
