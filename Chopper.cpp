#include "Chopper.h"
#include <iostream>
#include "SimulationException.h"

Chopper::Chopper(const std::string &name, const Point& pos) : Vehicle(name, 0, 0, pos){}

Chopper::Chopper(const std::string& name, const double speed, const int course, const Point& pos)
    : Vehicle(name, speed, course, pos) {}

void Chopper::set_parameters(const double speed, const double course) {
    if (speed > 0 && speed <= 170 && course >= 0 && course <= 360)
        Vehicle::set_parameters(speed, course);
}

// Command isn't supported.
void Chopper::set_course(const double course) {}

// Command isn't supported.
void Chopper::set_destination(const std::string &warehouse_name) {}

void Chopper::set_position(Point &pos) {
    Vehicle::set_position(pos);
}

int Chopper::get_stolen_crates() const {
    return stolen;
}

void Chopper::broadcast_current_state() const {
    std::cout << "Chopper " << get_name() << " at ";
    get_location().print();
    switch (get_status()) {
        case Stopped:
            std::cout<< ", Stopped"<<std::endl;
            break;
        case MovingOnCourse:
            std::cout << ", Heading on course " << get_course() <<
        " deg, speed " << get_speed() << " km/h" << std::endl;
        default:
            break;
    }
}

void Chopper::decrease_range() {
    if (range > 1) --range;
}

void Chopper::increase_range() {
    if (range < 20) ++range;
}

void Chopper::attack(Truck& target) {
    if (!is_in_range(target)) {     // Check if the target is within range.
        set_status(Stopped);
        decrease_range();
        throw AttackFailedException("Error: Truck is not in range, Attack failed");
    }

    // Check if cops are nearby (10KM)
    const bool cops_nearby = Model::get_instance().is_police_within_range(target.get_location());
    set_status(Stopped);
    if (cops_nearby) {
        decrease_range();
        throw TrooperNearbyException("Error: Troopers nearby, Attack failed");
    }

    // The Attack is successful.
    increase_range();
    stolen += target.unload();
    target.cancel_route();
    target.set_status(OffRoad);
}

bool Chopper::is_in_range(const Truck& target) const {
    // Checks if truck is within attack range.
    return calculate_distance(Vehicle::get_location(), target.get_location()) <= this->range;
}

void Chopper::queue_attack(const std::string& target, const int time) {
    Truck* aah = Model::get_instance().find_truck_by_name(target);
    if (is_in_range(*aah)) {
        attack(*aah);
        return;
    }
    for (const auto& attack_obj : attack_queue) {
        if (attack_obj.target == target) {
            return;
        }
    }
    attack_queue.push_back({aah->get_name(), time + 1});
}

void Chopper::update() {
    if (get_status() != Stopped)
        Vehicle::update();
    else {
        // If there are queued attacks, try to preform them.
        for (const auto& attack_obj : attack_queue) {
            Truck* target = Model::get_instance().find_truck_by_name(attack_obj.target);
            try {
                if (attack_obj.tick == Model::get_instance().get_time()) {
                    attack(*target);
                }
            }
            catch (const SimulationException& e) {
                std::cout << e.what() << std::endl;
            }
        }
        attack_queue.clear();
    }
}

