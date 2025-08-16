#ifndef SIMULATION_OBJECT_H
#define SIMULATION_OBJECT_H

#include <string>
#include "Geometry.h"

/**
 * Sim_Obj class
 * Abstract base class for all simulation objects.
 * Provides common interface: name, broadcasting, location retrieval, and update mechanism.
 */
class Sim_Obj{
public:
    explicit Sim_Obj(std::string _name);                // Constructor.
    std::string get_name() const;                       // Get object name.
    virtual void broadcast_current_state() const = 0;   // Broadcast state (pure virtual).
    virtual Point get_location() const = 0;             // Get location (pure virtual).
    virtual void update() = 0;                          // Update state (pure virtual).
    virtual ~Sim_Obj() = default;                       // Virtual destructor.

private:
    std::string name;                                   // Object name.
};

#endif //SIMULATION_OBJECT_H