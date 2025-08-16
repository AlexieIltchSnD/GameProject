# Vehicle Simulation System (MVC Design Pattern)

## Overview
This project implements a console-based vehicle simulation using the Model-View-Controller (MVC) architecture in C++.
It simulates a world of transportation and logistics, where various types of vehicles (trucks, state troopers, and choppers) interact with each other and with warehouses on a geographical map.
The system supports time-based simulation steps, command-driven vehicle creation and control, and ASCII-based visual representation.


## Features  
-  **MVC Architecture**: Clear separation between data (`Model`), display (`View`), and user interaction (`Controller`).
-  **Simulation Objects**:
-  `Truck`: Delivers crates between warehouses based on predefined schedules.
-  `StateTrooper`: Patrols between warehouses and can return to origin.
-  `Chopper`: Attacks trucks with a visible crate load.

-  **Command-line driven interface** with support for dynamic object creation, route assignments, and simulation control.
-  **ASCII Map View** of object locations and their statuses.
-  **Singleton Pattern**: Used in `Model` for centralized simulation state management.
-  **Object-Oriented Design Principles**: Includes use of inheritance, polymorphism, exception handling, and modern memory management (smart pointers).
-  **Simulation Mechanics**:
- Time advances in discrete steps (`go` command).
- Real-time status updates (`status` command).
- User-defined route loading from files.

## Project Structure
-  `main.cpp`: Entry point and REPL interface.
-  `Model`, `View`, `Controller`: Core MVC components.
-  `Vehicle`, `Truck`, `Chopper`, `StateTrooper`: Simulation actors.
-  `Warehouse`: Storage points for cargo.
-  `Sim_Obj`: Base class for simulation entities.
-  `Geometry`: Utilities for positions, directions, and calculations.
-  `CommandGenerator`: Parses and executes commands.
-  `Track_Base`: Support for routes and trip plans.
-  `Utils`, `Details`, `SimulationException`: Support classes for logic and error handling.

## Building the Project
This project uses modern C++ (C++11 and above).

### Compilation Example (using g++):
```bash
g++ -std=c++11 -Wall -Wextra -o vehicle *.cpp
```

## Running the Simulation
### Syntax:
```bash
./vehicle -w depot.txt -t Truck1.txt [Truck2.txt ...]
```
-  `-w`: Specifies the file containing warehouse definitions.
-  `-t`: Specifies one or more files with truck delivery schedules.

### Example:
```bash
./vehicle -w depot.dat -t Godzilla.txt
```

## Console Commands (in simulation)
-  `create <name> <type> <params>`: Create new vehicle (e.g. `create Cooper State_trooper Frankfurt`)
-  `go`: Advance simulation by one hour.
-  `status`: Print the status of all simulation objects.
-  `show`: Display ASCII map of the current simulation.
-  `exit`: Terminate the simulation.

Additional commands support modifying vehicle positions, courses, and performing actions such as `attack` or `stop`.

## Input File Formats
### Warehouse File (e.g. `depot.dat`)
```
<name>, (<x>, <y>), <inventory>
```

### Truck Schedule File (e.g. `Godzilla.txt`)
```
<warehouse_name>,<arrival_time>
<warehouse_name>,<arrival_time>,<crates>,<departure_time>
```

## Example
```bash
Time 0: Enter command: create Sylvester Chopper (14.00, 14.00)
Time 0: Enter command: Sylvester course 180 100
Time 0: Enter command: go
Time 1: Enter command: status
```

## Notes
- The simulation assumes a maximum of 12 character names for all objects.
- Vehicles may be prevented from moving depending on map logic or attack outcomes.
- Trucks can be attacked and lose all crates.
- All coordinates are in a 2D space with 100km max per axis.

## Authors
This project was submitted as part of the course  
**Advanced Topics in Object-Oriented Programming**  
Tel-Hai Academic College

- Student : Alexie Ilchuk – ID: 318210648
---