#include "Utils.h"
#include <iomanip>
#include <iostream>
#include "SimulationException.h"

std::vector<std::string> split_line(const std::string& line){
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;

    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }
    return tokens;
}

bool is_number(const std::string& s) {
    const std::regex pattern(NUMBER_FORMAT);
    return std::regex_match(s, pattern);
}

bool is_valid_sim_name(const std::string& warehouse) {
    const std::regex pattern(STRING_PATTERN);
    return !warehouse.empty() && std::regex_match(warehouse, pattern);
}

bool is_valid_truck_name(const std::string& truck_name) {
    const std::regex pattern(STRING_PATTERN);
    const size_t dot_pos = truck_name.find('.');    // Godzilla.txt
    if (dot_pos == std::string::npos) {
        return false;
    }
    return !truck_name.empty() && std::regex_match(truck_name.substr(0, dot_pos), pattern);
}

bool is_valid_time(const std::string& arrival, const std::string& departure) {
    const std::regex pattern(TIME_PATTERN);
    return std::regex_match(arrival, pattern) && std::regex_match(departure, pattern);
}

bool is_valid_coordinate_pair(const std::string& left, const std::string& right) {
    const std::regex pattern1(COORDINATE_LEFT);
    const std::regex pattern2(COORDINATE_RIGHT);
    return std::regex_match(left, pattern1) &&
           std::regex_match(right, pattern2);
}

void convert_tokens(std::vector<std::string>& tokens) {
    if (is_valid_sim_name(tokens[0])) {
        clean_strings(tokens);
        if (is_valid_coordinate_pair(tokens[1], tokens[2])) {
            if (is_number(tokens[3])) {
                tokens[1].erase(0,1);
                tokens[2].erase(tokens[2].size() - 1, 1);
            }else {
                throw InvalidArgumentException("Invalid number");
            }
        }else {
            throw InvalidArgumentException("Invalid coordinates");
        }
    }else {
        throw InvalidArgumentException("Invalid Warehouse name");
    }
}

bool is_valid_truck_line(const std::vector<std::string>& line) {
    return is_valid_sim_name(line[0]) && is_valid_time(line[1],line[3]) && is_number(line[2]);
}

void clean_strings(std::vector<std::string>& coordinates) {
    coordinates[1].erase(0, 1);
    coordinates[2].erase(0, 1);
    coordinates[3].erase(0, 1);
}

std::time_t create_time_for_ticks() {
    const std::string t_st = "01/01 00:00";
    std::tm tm = {};
    std::istringstream ss(t_st);
    ss >> std::get_time(&tm, TIME_FORMAT);
    const std::time_t now = std::time(nullptr);
    const std::tm today = *std::localtime(&now);
    tm.tm_year = today.tm_year;
    tm.tm_mon = today.tm_mon;
    tm.tm_mday = today.tm_mday;
    tm.tm_sec = 0;
    tm.tm_isdst = 1;
    const std::time_t temp =std::mktime(&tm);
    return temp;
}

int calculate_time_minutes(const std::time_t sim_time, const std::string &input) {
    const std::tm sim_tm = *std::localtime(&sim_time);

    std::tm time_tm = sim_tm;
    std::istringstream stream(input);
    stream >> std::get_time(&time_tm, "%H:%M");

    time_tm.tm_sec = 0;
    time_tm.tm_isdst = sim_tm.tm_isdst;

    const std::time_t input_time = std::mktime(&time_tm);

    const int diffMinutes = static_cast<int>(std::difftime(input_time, sim_time) / 60);
    return diffMinutes;
}

bool has_passed_target(const Point& from, const Point& target, const Point& current) {
    const double dx1 = target.x - from.x;
    const double dy1 = target.y - from.y;

    const double dx2 = target.x - current.x;
    const double dy2 = target.y - current.y;

    return (dx1 * dx2 + dy1 * dy2) < 0;
}

std::string trim(const std::string& str) {
    const auto start = str.find_first_not_of(" \t\r\n");
    const auto end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}