/**
 * Model class (Singleton)
 * Manages all simulation objects (vehicles, choppers, troopers, warehouses) and simulation state.
 * Provides functions to create, find, and control simulation objects, manage views, and update simulation time and state.
 */
#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <memory>
#include "Chopper.h"
#include "View.h"
#include "Warehouse.h"
#include "Utils.h"
#include <fstream>
#include "StateTrooper.h"
#include "Truck.h"

#define RANGE 10.0

class Chopper;
class Vehicle;
class Truck;
class Warehouse;

class Model {
public:
    static Model& get_instance();              // Get singleton instance.

    void add_sim_object(const std::shared_ptr<Sim_Obj>& obj); // Add a simulation object.
    void create_chopper(const std::string& name, float x, float y); // Create chopper.
    void create_trooper(const std::string& name, const Point& pos, const std::string& warehouse_name); // Create trooper.
    void set_chopper_course_and_speed(const std::string& name, double _course, double _speed) const;   // Set chopper course and speed.
    void set_trooper_course(const std::string& name, double _course) const;                            // Set trooper course.
    void set_trooper_destination(const std::string& name, const std::string& _destination) const;      // Set trooper destination.

    void queue_attack(const std::string &attacker, const std::string &target) const;  // Queue attack command, attacks if in range.
    void stop_vehicle(const std::string& vehicle_name) const;                   // Stop the vehicle.

    void load_depot_file(const std::string& file_name);       // Load depot file.
    void load_truck_file(const std::string& file_name);       // Load a truck file.

    StateTrooper* find_state_trooper_by_name(const std::string& trooper_name) const; // Find the trooper by name.
    Chopper* find_chopper_by_name(const std::string& chopper_name) const;            // Find chopper by name.
    Truck* find_truck_by_name(const std::string& truck_name) const;                  // Find truck by name.
    Vehicle* find_vehicle_by_name(const std::string& vehicle_name) const;            // Find vehicle by name.
    Warehouse* find_warehouse_by_name(const std::string& warehouse_name) const;      // Find warehouse by name.
    std::string get_warehouse_name_from_point(const Point& point) const;             // Get warehouse name from point.

    bool is_police_within_range(const Point& target) const;  // Check if troopers are within RANGE km of target.
    int get_time() const;                                    // Get simulation time.

    std::time_t get_sim_time() const;                        // Get system simulation time.
    std::list<std::shared_ptr<View>>& get_view_list();       // Get a list of views.
    std::list<std::shared_ptr<Sim_Obj>>& get_sim_list();     // Get a list of simulation objects.

    void broadcast_status() const;                           // Broadcast status to views.
    void attach(std::shared_ptr<View>& v);                   // Attach view.
    void detach(const std::shared_ptr<View>& v);             // Detach view.
    void notify_views() const;                               // Notify views.

    void update();                                           // Update simulation.
    void update_trucks() const;                              // Update trucks.
    void update_choppers_and_troopers() const;               // Update choppers and troopers.

    template<typename T>
    T* find_by_name(const std::string& obj_name) const {     // Find any object by name via type provided.
        for (const auto& obj_ptr : sim_obj_list) {
            auto target = std::dynamic_pointer_cast<T>(obj_ptr);
            if (target && obj_ptr->get_name() == obj_name) {
                return target.get();
            }
        }
        return nullptr;
    }

private:
    Model();                                 // Private constructor.
    Model(const Model&) = delete;            // Delete copy constructor.
    Model& operator=(const Model&) = delete; // Delete assignment operator.

    std::list<std::shared_ptr<Sim_Obj>> sim_obj_list; // List of simulation objects.
    std::list<std::shared_ptr<View>> view_list;       // List of attached views.

    int time = 0;                         // Simulation time.
    std::time_t sim_time;                // System time.
};

#endif //MODEL_H