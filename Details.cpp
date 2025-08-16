#include "Details.h"

Details::Details(std::string  _name, std::string _departure)
: location_name(std::move(_name)), case_quantity(0), departure_time(std::move(_departure)){}

Details::Details(std::string  _name, std::string  _arrival, const int cases, std::string  _departure)
: location_name(std::move(_name)), arrival_time(std::move(_arrival)), case_quantity(cases), departure_time(std::move(_departure)){}

std::string Details::get_location_name() const {
    return location_name;
}

std::string Details::get_arrival_time() const {
    return arrival_time;
}

std::string Details::get_departure_time() const {
    return departure_time;
}

int Details::get_case_quantity() const {
    return case_quantity;
}

void Details::set_case() {
    case_quantity = 0;
}

