#ifndef DETAILS_H
#define DETAILS_H

#include <string>

/**
 * Details class
 * Represents delivery or stop details including location, arrival and departure times, and case quantity.
 * Provides getters and methods to set case quantity.
 */
class Details {
public:
    explicit Details(std::string _name, std::string _departure);  // Constructor without arrival and case quantity.
    explicit Details(std::string _name, std::string _arrival, int cases, std::string _departure); // Constructor with arrival and case quantity.
    std::string get_location_name() const;       // Get location name.
    std::string get_arrival_time() const;        // Get arrival time.
    std::string get_departure_time() const;      // Get departure time.
    int get_case_quantity() const;               // Get case quantity.
    void set_case();                             // Mark case as handled/delivered.

private:
    std::string location_name;                   // Location name.
    std::string arrival_time;                    // Arrival time.
    int case_quantity;                           // Quantity of cases.
    std::string departure_time;                  // Departure time.
};
#endif //DETAILS_H
