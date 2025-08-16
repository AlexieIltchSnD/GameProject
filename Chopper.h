#ifndef CHOPPER_H
#define CHOPPER_H

#include "Model.h"
#include "Geometry.h"
#include "Vehicle.h"
#include "Truck.h"
#include "Utils.h"
#include <vector>

/**
 * Chopper class, extends Vehicle
 * Represents a chopper that can attack trucks, steal crates, and manage range.
 * Provides chopper-specific behaviors and overrides base Vehicle functions.
 */
class Chopper final : public Vehicle{
public:
    Chopper(const std::string &name,const Point& pos);
    Chopper(const std::string &name, double speed, int course,const Point& pos);

    void set_destination(const std::string &warehouse_name) override;  // Set chopper destination.
    void set_parameters(double speed, double course) override;         // Set speed and course.
    void set_course(double course) override;                           // Set course.
    void set_position(Point& pos) override;                            // Set position.
    void broadcast_current_state() const override;                     // Broadcast state.

    void decrease_range();              // Decrease chopper's range.
    void increase_range();              // Increase chopper's range.
    int get_stolen_crates() const;      // Get number of stolen crates.
    void attack(Truck& target);         // Attack a truck.
    bool is_in_range(const Truck& target) const; // Check if the truck is in attack range.
    void queue_attack(const std::string& target, int time);   // Queue an attack on a truck.

    void update() override;             // Update chopper state.

private:
    double range = 2;                              // Chopper range.
    int stolen = 0;                                // Number of stolen crates.
    std::vector<AttackCommand> attack_queue;       // List of queued attack targets.
};
#endif //CHOPPER_H