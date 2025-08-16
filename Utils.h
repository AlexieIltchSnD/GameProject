#ifndef UTILS_H
#define UTILS_H

#include <regex>
#include "Geometry.h"

#define MAX_STRING_LENGTH 12                                // Maximum string length.
#define STRING_PATTERN "^[a-zA-Z]{1,12}$"                   // String pattern (Only letters).
#define TIME_PATTERN "^\\d{2}:\\d{2}$"                      // Time Pattern ("10:00").
#define COORDINATE_LEFT "^\\(\\d+\\.\\d{2}$"                // Left coordinate. ("(10.30,")
#define COORDINATE_RIGHT "^\\d+\\.\\d{2}\\)$"               // Right coordinate. (" 42.22)")
#define TIME_FORMAT "%H:%M"                                 // Time Format ("H:M")
#define NUMBER_FORMAT "^-?\\d+\\.\\d+$|^-?\\d+$"            // Number Format. (-10.12,44.255,30)
#define ROBBER "Chopper"
#define POLICE "State_trooper"

// Struct that is used in delaying Chopper attacks to the next ticks if needed.
struct AttackCommand {
    std::string target;
    int tick;
};

// Function that splits a line with a delimiter of ',' returns a vector with separated words.
std::vector<std::string> split_line(const std::string& line);

// Function that checks if a given string is in the pattern of a number, defined in .h
// Returns true if it is, false otherwise.
bool is_number(const std::string& s);

// Function that checks if a given string is in a string pattern, defined in .h
// Returns true if it is, false otherwise.
bool is_valid_sim_name(const std::string& warehouse);

// Function that checks if a given filename is in a string pattern, defined in .h
// Returns true if it is, false otherwise.
bool is_valid_truck_name(const std::string& truck_name);

// Function that checks if 2 strings given are in the time pattern, defined in .h
// Returns true if they are, false otherwise.
bool is_valid_time(const std::string& arrival, const std::string& departure);

// Function that checks if 2 strings given are in the coordinate pattern, defined in .h
// Returns true if they are, false otherwise.
bool is_valid_coordinate_pair(const std::string& left, const std::string& right);

// Function that changes and checks the vector for its content, and changes
// the vector in place.
void convert_tokens(std::vector<std::string>& tokens);

// Function that receives a line from an input file, and returns true if the line is valid,
// returns false otherwise.
bool is_valid_truck_line(const std::vector<std::string>& line);

// "Cleans" the vector from non-needed parameters in place.
void clean_strings(std::vector<std::string>& coordinates);

// Function that returns the time "00:00" for the main simulation.
std::time_t create_time_for_ticks();

// Function that returns the difference between simulation time and a input string.
int calculate_time_minutes(std::time_t sim_time, const std::string &input);

// Function that returns true if the dot product between target and from, and target with current smaller than 0.
// Returns false otherwise.
bool has_passed_target(const Point& from, const Point& target, const Point& current);

// Function that takes a string, and trims the end. (/r, /n, /t, ' ')
std::string trim(const std::string& str);
#endif //UTILS_H