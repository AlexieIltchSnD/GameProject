#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <string>
#include "CommandGenerator.cpp"

/**
 * Controller class,
 * This class is responsible for controlling the Model and View according to interactions
 * with the user.
*/
class Controller {
public:
    Controller() = default;                     // Default constructor.
    ~Controller();                              // Destructor.

    void run(int argc, char *argv[]);           // Run simulation.
    void load(int argc, char * argv[]);         // Load data.
    void execute(const std::string& command);   // Execute a given command.

private:
    std::map<std::string, CommandFunction> commandsMap = buildCommandsMap(); // Map command strings to functions.
};
#endif //CONTROLLER_H
