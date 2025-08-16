#include "Controller.h"
#include <iostream>
#include "SimulationException.h"

void Controller::run(const int argc, char *argv[]) {
    std::cerr.rdbuf(std::cout.rdbuf());             // All cerr goes to cout, Eliminates print delay.
    try {
        load(argc, argv);                   // Load files.
    }
    catch (const SimulationException& e) {
        std::cerr << e.what() << std::endl; // On any error, exit.
        exit(1);
    }
    std::string command;
    while (true) {
        std::cout << "Time " << Model::get_instance().get_time() << ": Enter command: ";
        std::getline(std::cin, command);
        if (command == "exit") break;
        execute(command);                   // Execute user commands.
    }
}

void Controller::load(const int argc, char * argv[]) {
    if (argc < 4 || std::string(argv[1]) != "-w") {
        throw InvalidFileArgumentsException("Usage: –w depot.dat –t <truckfile1> [<truckfile2> <truckfile3> ...]");
    }
    const std::string depot_file = argv[2];
    std::vector<std::string> truck_files;
    if (std::string(argv[3]) == "-t") {
        for (int i = 4; i < argc; ++i)
            truck_files.emplace_back(argv[i]);
    } else {
        throw InvalidFlagsException("Usage: –w depot.dat –t <truckfile1> [<truckfile2> <truckfile3> ...]");
    }
    // Load all files from the model after receiving the correct flags.
    Model& model = Model::get_instance();
    model.load_depot_file(depot_file);
    for (const auto& tf : truck_files)
        model.load_truck_file(tf);
}

void Controller::execute(const std::string & command) {
    try {
        if (command.empty()) throw InvalidCommandException("Error: Command is empty");

        std::stringstream stream(command);
        std::vector<std::string> tokens;
        std::string token;
        while (stream >> token)         // Tokenize the command
            tokens.push_back(token);

        if (tokens.size() > 5) {
            throw InvalidCommandException("Error: Command contains too many arguments");
        }

        const auto command_at_token0 = commandsMap.find(tokens[0]);
        const auto command_at_token1 = (tokens.size() > 1) ? commandsMap.find(tokens[1]) : commandsMap.end();

        if (command_at_token0 != commandsMap.end()) {       // Command is at token 0 (create, show...)
            command_at_token0->second(tokens);
        }
        else if (command_at_token1 != commandsMap.end()) {  // Command is at token 1 (position, course...)
            command_at_token1->second(tokens);
        }else {
            throw InvalidCommandException("Error: Command not found");
        }
    }catch (SimulationException& e) {
        std::cerr << e.what() <<  std::endl;
    }
}

Controller::~Controller() = default;