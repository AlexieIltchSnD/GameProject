#include "Geometry.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include "Utils.h"

double to_radians(const double theta_d){
	return theta_d * pi / 180.0;
}

double to_degrees(const double theta_r){
	return theta_r * 180.0 / pi;
}

// construct a Cartesian_vector from a Polar_vector
Cartesian_vector::Cartesian_vector(const Polar_vector& pv) {
	delta_x = pv.r * cos(pv.theta);
	delta_y = pv.r * sin(pv.theta);
}

Cartesian_vector::Cartesian_vector(){
	delta_x = 0.0;
	delta_y = 0.0;
}

void Cartesian_vector::operator=(const Polar_vector& pv){
	delta_x = pv.r * cos(pv.theta);
	delta_y = pv.r * sin(pv.theta);
}

// construct a Polar_vector from a Cartesian_vector
Polar_vector::Polar_vector(const Cartesian_vector& cv) {
	r = sqrt((cv.delta_x * cv.delta_x) + (cv.delta_y * cv.delta_y));
	/* atan2 will return a negative angle for Quadrant III, IV, must translate to I, II */
	theta = atan2(cv.delta_y, cv.delta_x);
	if (theta < 0.)
		theta = 2. * pi + theta; // normalize theta positive
}

Polar_vector::Polar_vector(){
	r = 0.0;
	theta = 0.0;
}

void Polar_vector::operator=(const Cartesian_vector& cv){
	r = sqrt((cv.delta_x * cv.delta_x) + (cv.delta_y * cv.delta_y));
	/* atan2 will return a negative angle for Quadrant III, IV, must translate to I, II */
	theta = atan2(cv.delta_y, cv.delta_x);
	if (theta < 0.)
		theta = 2. * pi + theta; // normalize theta positive
}

Point::Point(const double x, const double y) : x(x), y(y) {}
Point::Point(){
	x = 0.0;
	y = 0.0;
}

void Point::print() const{
	cout << setprecision(2) << "(" << x << ", " << y << ")";
}

bool Point::operator==(const Point & rhs) const{
	return x == rhs.x && y == rhs.y;
}

double calculate_distance(const Point& a, const Point& b) {
	const double dx = b.x - a.x;
	const double dy = b.y - a.y;
	return std::sqrt(dx * dx + dy * dy);
}

/**
 * Calculate a course angle in degrees between two points.
 * Utility functions: trim string, compute time difference in minutes between two HH:MM time strings.
 */
double calculate_course_deg(const Point& a, const Point& b) {
	Cartesian_vector cv;
	cv.delta_x = b.x - a.x;
	cv.delta_y = b.y - a.y;
	const Polar_vector pv(cv);
	double corrected = 90.0 - to_degrees(pv.theta);
	if (corrected < 0)
		corrected += 360.0;
	return corrected;
}

int time_difference_minutes(const std::string& from, const std::string& to) {
	auto parse = [](const std::string& time_str) -> int {
		std::string trimmed = trim(time_str);  // trim first
		const size_t colon = trimmed.find(':');
		const int hours = std::stoi(trimmed.substr(0, colon));
		const int minutes = std::stoi(trimmed.substr(colon + 1));
		return hours * 60 + minutes;
	};
	return parse(trim(to)) - parse(trim(from));
}