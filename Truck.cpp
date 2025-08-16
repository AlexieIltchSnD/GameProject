#include "Truck.h"

#include <iostream>

#include "Model.h"

Truck::Truck(const std::string& name, const double speed, const double course, const Point& pos, const std::list<Details>& path)
    : Vehicle(name, speed, course, pos), truck_path(path) {}

// Sets the course for the truck, but cancels his route.
void Truck::set_course(const double course) {
    Vehicle::set_course(course);
    cancel_route();
}

void Truck::cancel_route() {
    truck_path.clear();
}

// Broadcast the truck state.
void Truck::broadcast_current_state() const {
    int crates = unload();
    std::string line;
    switch (get_status()) {
        case OffRoad:
            line = "Off road";
            crates = 0;
            break;
        case Stopped:
            line = "Stopped";
            break;
        case Parked:
            line = "Parked at " +truck_path.front().get_location_name();
            crates -= truck_path.front().get_case_quantity();
            break;
        default:
            line = "Heading to " + truck_path.front().get_location_name();
            break;
    }
    std::cout << "Truck " << get_name() << " at ";
    get_location().print();
    std::cout << ", " << line << ", Crates: " << crates << std::endl;
}

int Truck::unload() const {
    int stolen = 0;
    for (auto& stop : truck_path)
        stolen += stop.get_case_quantity();
    return stolen;
}

// Truck doesnt support this function.
void Truck::set_destination(const std::string& warehouse_name) {}

// Set the truck position, but cancels his route.
void Truck::set_position(Point& pos) {
    Vehicle::set_position(pos);
    cancel_route();
}

void Truck::update() {
    // Robbed, or stopped, or path ended return.
    if (get_status() == Stopped || get_status() == OffRoad || truck_path.empty())
        return;

    // Truck at warehouse, calculate the time for departure.
    const int minutes_to_departure = calculate_time_minutes(Model::get_instance().get_sim_time(),
                                                        truck_path.front().get_departure_time());
    if (get_status() == Parked) {   // Truck is parked at a warehouse.
        if (minutes_to_departure > 0) {
            return;
        }
        set_status(MovingTo);      // Advance the Truck path.
        truck_path.pop_front();
    }

    const std::string warehouse_name = truck_path.front().get_location_name();
    Warehouse* wh = Model::get_instance().find_warehouse_by_name(warehouse_name);

    const Point prev_pos = get_location();

    set_speed(get_speed() * 100);
    Vehicle::update();
    set_speed(get_speed() / 100);

    // Check if truck passed the warehouse target.
    if (has_passed_target(prev_pos, wh->get_location(), get_location())) {
        // Forcefully park the truck, and update warehouse inventory.
        set_status(Parked);
        Point from(wh->get_location());
        Vehicle::set_position(from);
        wh->update_inventory(truck_path.front().get_case_quantity(),true);

        // Check if the truck has another warehouse.
        if (!truck_path.empty()) {
            const Details& front = truck_path.front();
            truck_path.pop_front();

            // Current is last stop of the truck.
            if (truck_path.empty()) {
                set_status(Stopped);
                truck_path.push_front(front);
                return;
            }

            // Set the next destination for the truck.
            const std::string drive_to = truck_path.front().get_location_name();
            const Warehouse* destination = Model::get_instance().find_warehouse_by_name(drive_to);
            const Point to = destination->get_location();

            // Calculate new speed, with course.
            const double dist = calculate_distance(from, to);
            const int mins = time_difference_minutes(front.get_departure_time(), front.get_arrival_time());
            const double speed = dist / (mins / 60.0);
            const double course = calculate_course_deg(from, to);
            set_parameters(speed, course);

            truck_path.push_front(front);
        } else {
            set_status(Stopped);
        }
    }
}
