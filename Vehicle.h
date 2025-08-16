#ifndef VEHICLE_H
#define VEHICLE_H

#include "Sim_Obj.h"
#include "Track_Base.h"

/**
 * Vehicle class, extends Sim_obj
 * Each vehicle (truck, chopper, trooper) extends this base class.
 * Contains basic virtual and non-virtual functions.
 */
class Vehicle : public Sim_Obj{
public:
    enum {Stopped,Parked,OffRoad,MovingOnCourse,MovingTo};  // All vehicle states.
    explicit Vehicle(const std::string& name, double speed, double course, const Point& position);

    virtual void set_destination(const std::string &warehouse_name) = 0; // Virtual set vehicle destination.
    virtual void set_parameters(double speed, double course);            // Set vehicle parameters.
    virtual void set_course(double course);                              // Set course.
    virtual void set_position(Point& pos);                               // Set vehicle position.
    void broadcast_current_state() const override = 0;                   // Virtual broadcast state , overridden from Sim_obj.

    void set_speed(double speed);   // Speed setter.
    void set_status(int _status);   // Status setter (vehicle state).

    Point get_location() const override;    // Location getter, overridden from Sim_obj
    int get_status() const;                 // Status getter.
    double get_speed() const;               // Speed getter.
    double get_course() const;              // Course getter.
    bool is_stopped() const;                // Is the vehicle in stopped state?
    std::string get_status_string() const;  // Get status string (vehicle state).

    void update() override;                 // Updates vehicle state, overridden from Sim_obj
    ~Vehicle() override = default;          // Destructor.

private:
    int status = Stopped;                   // Vehicle status.
    Track_Base base;                        // All basic information (speed, course, location) of a vehicle.
};
#endif //VEHICLE_H