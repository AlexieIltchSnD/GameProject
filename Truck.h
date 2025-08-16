#ifndef TRUCK_H
#define TRUCK_H

#include <list>
#include "Vehicle.h"
#include "Details.h"

/**
 * Truck class, extends Vehicle
 * Represents a truck moving between warehouses on a predefined path.
 * Overrides base Vehicle functions and manages a path of deliveries.
 */
class Truck final : public Vehicle{
public:
    Truck(const std::string& name, double speed, double course, const Point& pos, const std::list<Details>& path);

    void set_destination(const std::string& warehouse_name) override; // Set truck destination, overridden from Vehicle.
    void set_course(double course) override;                          // Set course, overridden from Vehicle.
    void set_position(Point& pos) override;                           // Set position, overridden from Vehicle.
    void broadcast_current_state() const override;                    // Broadcast truck state, overridden from Vehicle.

    void cancel_route();                // Cancel truck route.
    int unload() const;                 // Count all cargo to unload.
    void update() override;             // Update truck state.

private:
    std::list<Details> truck_path;  // Path of warehouses to visit.
};

#endif //TRUCK_H