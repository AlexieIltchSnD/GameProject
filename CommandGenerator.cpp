#include <functional>
#include "Model.h"
#include "SimulationException.h"

using CommandFunction = std::function<void(std::vector<std::string>& parameters)>;
inline std::map<std::string, CommandFunction> buildCommandsMap() {
    std::map<std::string,CommandFunction> commandsMap;

    // Create command, check every field given, and call Model for insertion.
    // Throws SimulationException upon bad input.
    commandsMap["create"] = [&](std::vector<std::string>& parameters) {
        if (parameters.size() < 4) throw InvalidCommandException("Error: Create receives 4-5 arguments");
        if (!is_valid_sim_name(parameters[1])) throw IllegalNameException("Error: Vehicle name is not valid");

        if (parameters.size() == 5 && parameters[2] == ROBBER) {    // Chopper found.
            if (parameters[3].back() == ',') {
                parameters[3].pop_back();
            }else
                throw BadCoordinatesException("Error: Wrong coordinates format");

            if (is_valid_coordinate_pair(parameters[3], parameters[4])) {
                parameters[3].erase(0,1);
                parameters[4].erase(parameters[4].size() - 1, 1);
                const float ix = std::stof(parameters[3]);
                const float iy = std::stof(parameters[4]);
                Model::get_instance().create_chopper(parameters[1], ix, iy);    // Call chopper insertion.
            }else {
                throw BadCoordinatesException("Error: Wrong coordinates format");
            }
        }
        else if (parameters.size() == 4 && parameters[2] == POLICE) {   // Trooper found.
                const auto* warehouse = Model::get_instance().find_warehouse_by_name(parameters[3]);
                if (!warehouse)
                    throw WarehouseNotFoundException("Error: Warehouse not found in database");
            Model::get_instance().create_trooper(parameters[1], warehouse->get_location(), parameters[3]);  // Call trooper insertion.
        }else {
            throw InvalidCommandFormatException("Error: Wrong command for create");
        }
    };

    // Course command, check every field given, and call Model for changes.
    // Throws SimulationException upon bad input.
    commandsMap["course"] = [&](const std::vector<std::string>& parameters) {
        Vehicle* target = Model::get_instance().find_vehicle_by_name(parameters[0]);
        if (!target)
            throw VehicleNotFoundException("Error: Vehicle <" + parameters[0] + "> not found in database");

        const auto chopper = dynamic_cast<Chopper*>(target);
        if (chopper) {                  // Chopper found.
            if (parameters.size() != 4)
                throw InvalidCommandFormatException("Error: Expected 4 parameters for Chopper course command");

            if (!is_number(parameters[2]) || !is_number(parameters[3]))
                throw InvalidArgumentsForTypeException("Error: Speed / Course are not numbers");

            const double course = std::stod(parameters[2]);
            const double speed = std::stod(parameters[3]);
            if (course >= 0 && course <= 360) {
                Model::get_instance().set_chopper_course_and_speed(parameters[0], course, speed);   // Set speed and course.
                return;
            }
            throw InvalidArgumentException("Error: Course is invalid");
        }
        const auto trooper = dynamic_cast<StateTrooper*>(target);
        if (trooper) {                  // Trooper found.
            if (parameters.size() != 3)
                throw InvalidCommandFormatException("Error: Expected 3 parameters for Trooper course command");

            if (!is_number(parameters[2]))
                throw InvalidArgumentsForTypeException("Error: Course is not a number");

            const double course = std::stod(parameters[2]);
            if (course >= 0 && course <= 360) {
                Model::get_instance().set_trooper_course(parameters[0], course);        // Set course for trooper.
                return;
            }
            throw InvalidArgumentException("Error: Course is invalid");
        }
        throw InvalidCommandFormatException("Error: Wrong command for course");
    };

    // Position command, check every field given, and change fields in place.
    // Throws SimulationException upon bad input.
    commandsMap["position"] = [&](std::vector<std::string>& parameters) {
        if (parameters.size() < 4)
            throw InvalidCommandFormatException("Error: Not enough parameters for position command");

        const Model& model = Model::get_instance();
        Vehicle* target = model.find_vehicle_by_name(parameters[0]);
        if (!target)
            throw VehicleNotFoundException("Error: Vehicle <" + parameters[0] + "> not found in database");

        if (parameters[2].back() != ',')
            throw BadCoordinatesException("Error: Wrong coordinates format");

        parameters[2].pop_back();
        if (!(is_valid_coordinate_pair(parameters[2], parameters[3])))
            throw BadCoordinatesException("Error: Wrong coordinates format");

        parameters[2].erase(0, 1);
        parameters[3].erase(parameters[3].size() - 1, 1);
        const float x = std::stof(parameters[2]);
        const float y = std::stof(parameters[3]);
        Point pos(x, y);

        if (parameters.size() == 4) {                           // Truck or trooper.
            if (auto* truck = dynamic_cast<Truck*>(target))
                truck->set_position(pos);
            else if (auto* trooper = dynamic_cast<StateTrooper*>(target))
                trooper->set_position(pos);                 // Set position.
            else
                throw InvalidArgumentsForTypeException("Error: Vehicle is not Truck or StateTrooper");
        }
        else if (parameters.size() == 5 && is_number(parameters[4])) {  // Chopper.
                if (auto* chopper = dynamic_cast<Chopper*>(target)) {
                    const double speed = std::stod(parameters[4]);
                    if (speed >= 0 && speed <= 170) {
                        chopper->set_speed(speed);
                        chopper->set_position(pos);                 // Set speed and position.
                        chopper->set_status(Vehicle::MovingTo);
                    }
                    else
                        throw InvalidArgumentException("Error: Speed is invalid");
                }
                else
                    throw InvalidCommandFormatException("Error: Vehicle is not Chopper");
            }
        else
            throw InvalidCommandFormatException("Error: Wrong command format for position");

        };

    // Destination command, trooper only command, call Model for changes.
    // Throws SimulationException upon bad input.
    commandsMap["destination"] = [&](const std::vector<std::string>& parameters) {
        if (parameters.size() != 3)
            throw InvalidCommandFormatException("Error: Destination receives 2 arguments");

        Model::get_instance().set_trooper_destination(parameters[0], parameters[2]);
    };

    // Attack command, check every field given, and attack a truck if possible via Model.
    // Otherwise, queue the attack for the time tick.
    // Throws SimulationException upon bad input.
    commandsMap["attack"] = [&](const std::vector<std::string> &parameters) {
        if (parameters.size() != 3)
            throw InvalidCommandFormatException("Error: Attack receives 2 arguments");

        const Chopper* chopper = Model::get_instance().find_chopper_by_name(parameters[0]);
        const Truck* truck = Model::get_instance().find_truck_by_name(parameters[2]);
        if (!chopper)
            throw NotFoundException("Error: Chopper not found");

        if (!truck)
            throw NotFoundException("Error: Truck not found");

        Model::get_instance().queue_attack(parameters[0],parameters[2]);
    };

    // Stop command, check the existence of a vehicle via Model, if exists stop the vehicle.
    // Throws SimulationException upon bad input.
    commandsMap["stop"] = [&](const std::vector<std::string>& parameters) {
        if (parameters.size() != 2)
            throw InvalidCommandFormatException("Error: Size receives 1 argument only");

        Model::get_instance().stop_vehicle(parameters[0]);
    };

    // Go command, updates every object inside the Model via one tick time.
    // Throws SimulationException upon bad input.
    commandsMap["go"] = [&](const std::vector<std::string>& parameters) {
        if (parameters.size() != 1)
            throw InvalidCommandFormatException("Error: Go receives 0 arguments");

        Model::get_instance().update();
    };

    // Status command, receives no arguments, activates the broadcast_status function on
    // Every object inside the Model.
    // Throws SimulationException upon bad input.
    commandsMap["status"] = [&](const std::vector<std::string>& parameters) {
        if (parameters.size() != 1)
            throw InvalidCommandFormatException("Error: Status receives 0 arguments");

        Model::get_instance().broadcast_status();
    };

    // Default command, changes the View fields into the default configuration.
    // Throws SimulationException upon bad input.
    commandsMap["default"] = [&](const std::vector<std::string>& parameters) {
        if (parameters.size() != 1)
            throw InvalidCommandFormatException("Error: Default receives 0 arguments");

        for (const auto& view : Model::get_instance().get_view_list()) {
            view->defaults();
        }
    };

    // Size command, changes the size field of the View.
    // Throws SimulationException upon bad input.
    commandsMap["size"] = [&](const std::vector<std::string>& parameters) {
        if (parameters.size() != 2)
            throw InvalidCommandFormatException("Error: Size receives 1 argument only");

        if (is_number(parameters[1])) {
            const int new_size = std::stoi(parameters[1]);
            for (const auto& view : Model::get_instance().get_view_list()) {
                view->set_size(new_size);
            }
        }
        else
            throw InvalidArgumentException("Error: Argument is not a number");
    };

    // Zoom command, changes the scale field of the View.
    // Throws SimulationException upon bad input.
    commandsMap["zoom"] = [&](const std::vector<std::string>& parameters) {
        if (parameters.size() != 2)
            throw InvalidCommandFormatException("Error: Zoom receives 1 argument only");

        if (!is_number(parameters[1]))
            throw InvalidArgumentException("Error: Zoom expects a double");

        const double new_zoom = std::stod(parameters[1]);
        for (const auto& view : Model::get_instance().get_view_list()) {
            view->zoom(new_zoom);
        }
    };

    // Pan command, changes the pan of the map inside View.
    // Throws SimulationException upon bad input.
    commandsMap["pan"] = [&](const std::vector<std::string>& parameters) {
        if (parameters.size() != 3)
            throw InvalidCommandFormatException("Error: Pan receives 2 coordinates only");

        if (!(is_number(parameters[1]) && is_number(parameters[2])))
            throw InvalidArgumentException("Error: Pan receives 2 doubles");

        const float x = std::stof(parameters[1]);
        const float y = std::stof(parameters[2]);
        const Point p(x, y);

        for (const auto& view : Model::get_instance().get_view_list()) {
            view->pan(p);
        }
    };

    // Show command, prints the map of the game inside the console.
    // Throws SimulationException upon bad input.
    commandsMap["show"] = [&](const std::vector<std::string>& parameters) {
       if (parameters.size() != 1)
           throw InvalidCommandFormatException("Error: Show receives 0 arguments");

       Model::get_instance().notify_views();
    };

    return commandsMap;
}