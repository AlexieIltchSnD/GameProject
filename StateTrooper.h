#ifndef STATETROOPER_H
#define STATETROOPER_H

#include <set>
#include "Vehicle.h"

/**
 * StateTrooper class, extends Vehicle
 * Represents a state trooper that moves between warehouses and tracks visited ones.
 * Overrides base Vehicle functions to provide trooper-specific behavior.
 */
class StateTrooper final : public Vehicle{
public:
    StateTrooper(const std::string &name, const Point& pos, std::string starting_warehouse); // Constructor.

    void set_destination(const std::string &warehouse_name) override; // Set destination warehouse.
    void set_parameters(double speed, double course) override;        // Set speed and course.
    void set_course(double course) override;                          // Set course.
    void set_position(Point &pos) override;                           // Set position.
    void broadcast_current_state() const override;                    // Broadcast state.

    void update() override;                                           // Update trooper state.

private:
    Point destination_point;             // Current destination point.
    bool has_destination = false;        // Flag indicating if destination is set.
    set<std::string> visited_warehouses; // Set of warehouses visited.
    std::string origin_warehouse_name;   // Starting warehouse name.
};

#endif //STATETROOPER_H