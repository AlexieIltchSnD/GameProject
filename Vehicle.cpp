#include "Vehicle.h"

// Vehicle Constructor.
Vehicle::Vehicle(const std::string& name, const double speed, const double course, const Point& position)
: Sim_Obj(name), base(course, speed, position) {}

// Sets the vehicle parameters via Track_Base private field.
void Vehicle::set_parameters(const double speed, const double course) {
    base.set_parameters(speed,course);
}

// Sets the vehicle course via Track_Base private field.
void Vehicle::set_course(const double course) {
    base.set_course(course);
}

// Sets the vehicle position via Track_Base private field.
void Vehicle::set_position(Point &pos) {
    base.set_position(pos);
}

// Sets the vehicle speed via Track_Base private field.
void Vehicle::set_speed(const double speed) {
    base.set_speed(speed);
}

// Sets the vehicle status.
void Vehicle::set_status(const int _status) {
    status = _status;
}

// Returns the vehicle position via Track_base private field.
Point Vehicle::get_location() const {
    return base.get_position();
}

// Returns the status of the current vehicle.
int Vehicle::get_status() const {
    return status;
}

// Returns the vehicle speed via Track_base private field.
double Vehicle::get_speed() const {
    return base.get_speed();
}

// Returns the vehicle course via Track_base private field.
double Vehicle::get_course() const {
    return base.get_course();
}

// Returns true if vehicle status is Stopped, false otherwise.
bool Vehicle::is_stopped() const {
    return status == Stopped;
}

// Returns the vehicle status as a string.
std::string Vehicle::get_status_string() const {
    switch (status) {
        case Stopped: return "Stopped";
        case OffRoad: return "Off road";
        case MovingOnCourse: return "Heading on course";
        case MovingTo: return "Heading to";
        case Parked: return "Parked";
        default: return "Unknown";
    }
}

// Updates vehicle position on a map, calculate the next location of the vehicle and add the points
// To the current position.
void Vehicle::update() {
    const double angle_rad = base.get_course() * pi / 180.0;
    const double dx = (base.get_speed() / 100.0) * sin(angle_rad);
    const double dy = (base.get_speed() / 100.0) * cos(angle_rad);
    base.add_position(dx, dy);
}