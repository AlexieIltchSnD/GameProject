#include "StateTrooper.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include "Model.h"

StateTrooper::StateTrooper(const std::string &name, const Point& pos, std::string  starting_warehouse)
    : Vehicle(name, 90.0, 0, pos), origin_warehouse_name(std::move(starting_warehouse)) {
    visited_warehouses.emplace(starting_warehouse);
}

void StateTrooper::set_parameters(const double speed, const double course) {
    if (course >= 0 && course <= 360 && speed == 90)
        Vehicle::set_parameters(speed, course);
}

void StateTrooper::set_destination(const std::string &warehouse_name) {
    const Warehouse* dest = Model::get_instance().find_warehouse_by_name(warehouse_name);
    if (!dest)
        throw std::invalid_argument("Error: Warehouse not found");

    destination_point = dest->get_location();
    set_course(calculate_course_deg(get_location(), destination_point));
    set_speed(90.0);
    set_status(MovingTo);
    has_destination = true;
}

void StateTrooper::set_position(Point &pos) {
    const double course = calculate_course_deg(pos, destination_point);
    Vehicle::set_course(course);
    destination_point = pos;
    set_status(MovingTo);
    has_destination = true;
}

void StateTrooper::set_course(const double course) {
    if (course >= 0 && course <= 360) {
        Vehicle::set_course(course);
        set_status(MovingOnCourse);
    }
}

void StateTrooper::broadcast_current_state() const {
    const std::string warehouse_name = Model::get_instance().get_warehouse_name_from_point(destination_point);
    std::string dest;

    if (!warehouse_name.empty())
        dest = warehouse_name;
    else {
        std::ostringstream oss;
        oss << "(" << std::fixed << std::setprecision(2)
            << destination_point.x << ", " << destination_point.y << ")";
        dest = oss.str();
    }

    std::cout << "State_trooper " << get_name() << " at ";
    get_location().print();
    switch (get_status()) {
        case Stopped:
            std::cout << ", Stopped" << std::endl;
            break;
        case MovingTo:
            std::cout << ", " << get_status_string() << " " << dest
                      << ", speed " << get_speed() << " km/h" << std::endl;
            break;
        default:
            break;
    }
}

void StateTrooper::update() {
    if (get_status() == Stopped || !has_destination)    // if a trooper is stopped or doesn't have a destination return.
        return;

    const Point prev = get_location();

    Vehicle::update();

    // if trooper didnt pass destination, return.
    if (!has_passed_target(prev, destination_point, get_location()))
        return;

    // Forcefully stop the trooper.
    Vehicle::set_position(destination_point);
    has_destination = false;

    // Check what warehouse trooper arrived to and insert him into the set.
    const std::string arrived_name = Model::get_instance().get_warehouse_name_from_point(destination_point);
    if (!arrived_name.empty())
        visited_warehouses.insert(arrived_name);

    // Full warehouse rotation finished, start another one.
    if (arrived_name == origin_warehouse_name) {
        visited_warehouses.clear();
        visited_warehouses.insert(origin_warehouse_name);
    }

    const auto& sim_objs = Model::get_instance().get_sim_list();
    Warehouse* next = nullptr;
    double min_dist = numeric_limits<double>::max();

    // Find the next closest warehouse that was unvisited.
    for (const auto& obj : sim_objs) {
        auto warehouse = std::dynamic_pointer_cast<Warehouse>(obj);
        if (!warehouse) continue;

        const std::string warehouse_name = warehouse->get_name();

        // Skip already visited
        if (visited_warehouses.count(warehouse_name)) continue;

        const double dist = calculate_distance(get_location(), warehouse->get_location());

        if (dist < min_dist || (abs(dist - min_dist) < 1e-6 && warehouse_name < next->get_name())) {
            min_dist = dist;
            next = warehouse.get();
        }
    }
    // The next destination found, go to the next warehouse.
    if (next) {
        destination_point = next->get_location();
        has_destination = true;
        const double angle = calculate_course_deg(get_location(), destination_point);
        set_course(angle);
        set_speed(90.0);
        set_status(MovingTo);
    }
}