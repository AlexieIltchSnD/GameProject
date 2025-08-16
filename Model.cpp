#include "Model.h"
#include <iostream>
#include "Details.h"
#include "SimulationException.h"

Model::Model() {
    sim_time = create_time_for_ticks(); // Create simulation time -> 00:00
    auto default_view = std::make_shared<View>();
    attach(default_view);
    const auto _default = std::make_shared<Warehouse>("Frankfurt",100000 ,40, 10);  // Default warehouse.
    _default->mark_main_warehouse();
    sim_obj_list.emplace_back(std::static_pointer_cast<Sim_Obj>(_default));
}

Model& Model::get_instance() {
    static Model instance;
    return instance;
}

void Model::add_sim_object(const std::shared_ptr<Sim_Obj>& obj) {
    sim_obj_list.push_back(obj);
}

void Model::create_chopper(const std::string& name, const float x, const float y) {
    const auto chopper = std::make_shared<Chopper>(name, Point(x, y));
    add_sim_object(chopper);
}

void Model::create_trooper(const std::string& name, const Point& pos, const std::string& warehouse_name) {
    const auto trooper = std::make_shared<StateTrooper>(name, pos,warehouse_name);
    add_sim_object(trooper);
}

void Model::set_chopper_course_and_speed(const std::string& name, const double _course, const double _speed) const {
    Chopper* chopper = find_chopper_by_name(name);
    if (!chopper)
        throw VehicleNotFoundException("Error: Chopper " + name + " not found");

    chopper->set_parameters(_speed,_course);
    chopper->set_status(Vehicle::MovingOnCourse);
}

void Model::set_trooper_course(const std::string& name, const double _course) const {
    StateTrooper* trooper = find_state_trooper_by_name(name);
    if (!trooper)
        throw VehicleNotFoundException("Error: StateTrooper " + name + " not found");

    trooper->set_course(_course);
}

void Model::set_trooper_destination(const std::string& name, const std::string& _destination) const {
    StateTrooper* state_trooper = find_state_trooper_by_name(name);
    Truck* truck = find_truck_by_name(_destination);
    if (truck) {
        truck->set_destination(_destination);
        return;
    }
    if (!state_trooper)
        throw NotFoundException("Error: Trooper not found");

    state_trooper->set_destination(_destination);
}

void Model::queue_attack(const std::string& attacker, const std::string& target) const {
    Chopper* apache_attack_helicopter = find_chopper_by_name(attacker);
    const Truck* truck = find_truck_by_name(target);
    if (!apache_attack_helicopter)
        throw VehicleNotFoundException("Error: Attacker not found");
    if (!truck)
        throw VehicleNotFoundException("Error: Target not found");

    if (truck->get_status() == Vehicle::OffRoad || truck->get_status() == Vehicle::Parked) return;

    apache_attack_helicopter->queue_attack(target,time);
}

void Model::stop_vehicle(const std::string& vehicle_name) const {
    Vehicle* vehicle = find_vehicle_by_name(vehicle_name);

    if (!vehicle)
        throw VehicleNotFoundException("Error: Vehicle <" + vehicle_name + "> not found in database");

    vehicle->set_status(Vehicle::Stopped);
}

void Model::load_depot_file(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        throw FileException("Error: Could not open file <" + file_name + ">");
    }

    std::string line;
    while(std::getline(file, line)) {
        line = trim(line);
        auto tokens = split_line(line);
        convert_tokens(tokens);
        float x = std::stof(tokens[1]);
        float y = std::stof(tokens[2]);
        int inventory = std::stoi(tokens[3]);
        if (find_warehouse_by_name(tokens[0]) != nullptr) continue;

        auto warehouse = std::make_shared<Warehouse>(tokens[0],inventory ,x, y);
        sim_obj_list.emplace_back(std::static_pointer_cast<Sim_Obj>(warehouse));
    }
    file.close();
}

void Model::load_truck_file(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file.is_open())
        throw FileException("Error: Could not open file <" + file_name + ">");

    if (!is_valid_truck_name(file_name))
        throw IllegalNameException("Error: Truck name is not valid");

    int accumulator = 0, cases = 0;     // total cases in file, cases each iteration.
    std::string line;
    std::list<Details> truck_path;
    std::string source, departure_time;
    std::getline(file, line);

    line = trim(line);
    auto tokens = split_line(line);

    if (find_warehouse_by_name(tokens[0]) == nullptr)
        throw WarehouseNotFoundException("Error: Warehouse <" + tokens[0] + "> not found");

    if (!is_valid_time(tokens[1], "00:00"))
        throw InvalidInputLineException("Error: Wrong file format");

    if (tokens.size() != 2)
        throw InvalidInputLineException("Error: First line must contain 2 arguments");

    source = tokens[0], departure_time = tokens[1];

    while (std::getline(file, line)) {
        line = trim(line);
        tokens = split_line(line);

        if (find_warehouse_by_name(tokens[0]) == nullptr)
            throw WarehouseNotFoundException("Error: Warehouse <" + tokens[0] + "> not found");

        if (tokens.size() != 4)
            throw InvalidInputLineException("Error: Every other line must contain 4 arguments");

        if (!is_valid_truck_line(tokens))
            throw InvalidInputLineException("Error: Wrong file format");

        accumulator += std::stoi(tokens[2]);
        truck_path.emplace_back(source,tokens[1], cases ,departure_time);
        cases = std::stoi(tokens[2]);

        source = tokens[0], departure_time = tokens[3];
    }
    truck_path.emplace_back(source,"00:00",cases,departure_time);
    std::string truck_name = file_name.substr(0, file_name.find('.'));

    auto it = ++truck_path.begin();
    const Point p0 = find_warehouse_by_name(truck_path.front().get_location_name())->get_location();
    const Point p1 = find_warehouse_by_name(it->get_location_name())->get_location();

    find_warehouse_by_name(truck_path.front().get_location_name())->update_inventory(accumulator,false);

    double dist = calculate_distance(p0, p1);
    int mins = time_difference_minutes(truck_path.front().get_departure_time(), truck_path.front().get_arrival_time());
    double speed = dist / (mins / 60.0);
    double course = calculate_course_deg(p0, p1);

    truck_path.pop_front();

    auto truck = std::make_shared<Truck>(truck_name, speed, course, p0, truck_path);
    truck->set_status(Vehicle::MovingTo);
    sim_obj_list.emplace_back(std::static_pointer_cast<Sim_Obj>(truck));
}

StateTrooper* Model::find_state_trooper_by_name(const std::string &trooper_name) const {
    return find_by_name<StateTrooper>(trooper_name);
}

Chopper* Model::find_chopper_by_name(const std::string &chopper_name) const {
    return find_by_name<Chopper>(chopper_name);
}

Truck* Model::find_truck_by_name(const std::string& truck_name) const {
    return find_by_name<Truck>(truck_name);
}

Vehicle* Model::find_vehicle_by_name(const std::string& vehicle_name) const {
    return find_by_name<Vehicle>(vehicle_name);
}

Warehouse* Model::find_warehouse_by_name(const std::string& warehouse_name) const {
    return find_by_name<Warehouse>(warehouse_name);
}

bool Model::is_police_within_range(const Point& target) const {
    for (const auto& obj : sim_obj_list) {
        const auto trooper = std::dynamic_pointer_cast<StateTrooper>(obj);
        if (trooper && calculate_distance(trooper->get_location(), target) <= RANGE) {
            return true;
        }
    }
    return false;
}

int Model::get_time() const {
    return time;
}

std::time_t Model::get_sim_time() const {
    return sim_time;
}

std::list<std::shared_ptr<View>> & Model::get_view_list() {
    return view_list;
}

std::list<std::shared_ptr<Sim_Obj>> & Model::get_sim_list() {
    return sim_obj_list;
}

void Model::broadcast_status() const {
    for (const auto& obj : sim_obj_list) {
        obj->broadcast_current_state();
    }
}

void Model::attach(std::shared_ptr<View>& v) {
    view_list.emplace_back(std::move(v));
}

void Model::detach(const std::shared_ptr<View>& v) {
    view_list.remove_if([&](const std::shared_ptr<View>& ptr) {
            return ptr == v;
        });
}

void Model::notify_views() const {
    for (const auto& v : view_list) {
        v->show();
    }
}

void Model::update(){
    ++time;
    update_trucks();
    update_choppers_and_troopers();
    sim_time += static_cast<time_t>(3600);
}

void Model::update_trucks() const {
    for (const auto& sim_obj : sim_obj_list) {
        if (const auto truck = std::dynamic_pointer_cast<Truck>(sim_obj)) {
            if (truck->get_status() == Vehicle::Stopped || truck->get_status() == Vehicle::OffRoad)
                continue;
            sim_obj->update();
        }
    }
}

void Model::update_choppers_and_troopers() const {
    for (const auto& sim_obj : sim_obj_list) {
        if (std::dynamic_pointer_cast<Chopper>(sim_obj)) {
            const auto chopper = std::dynamic_pointer_cast<Chopper>(sim_obj);
            chopper->update();
        }else if (std::dynamic_pointer_cast<StateTrooper>(sim_obj)){
            const auto trooper = std::dynamic_pointer_cast<StateTrooper>(sim_obj);
            trooper->update();
        }
    }
}

std::string Model::get_warehouse_name_from_point(const Point& point) const {
    for (const auto& obj_ptr : sim_obj_list) {
        if (std::dynamic_pointer_cast<Warehouse>(obj_ptr)) {
            const auto warehouse = std::dynamic_pointer_cast<Warehouse>(obj_ptr);
            Point warehouse_point = warehouse->get_location();
            if (warehouse_point == point)
                return warehouse->get_name();
        }
    }
    return "";
}