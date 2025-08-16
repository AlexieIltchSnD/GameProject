#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <ctgmath>

/**
 * Geometry utilities and structures
 * Provides Point, Cartesian_vector, Polar_vector structures, and conversion utilities.
 * Also includes distance and course calculation helpers.
 */
using namespace std;
const double pi = 2. * atan2(1., 0.);

// angle units conversion functions
double to_radians(double theta_d);
double to_degrees(double theta_r);

// class declarations
typedef struct Point {
	double x;
	double y;
	Point(double x, double y);
	Point();
	void print() const;
	bool operator==(const Point& rhs) const;
} Point;

// Polar and Cartesian vector structures
struct Polar_vector;
typedef struct Cartesian_vector {
	explicit Cartesian_vector(const Polar_vector & pv);
	Cartesian_vector();
	void operator=(const Polar_vector & pv);
	double delta_x;
	double delta_y;
} Cartesian_vector;

typedef struct Polar_vector {
	explicit Polar_vector(const Cartesian_vector & cv);
	Polar_vector();
	void operator=(const Cartesian_vector & cv);
	double r;
	double theta;
} Polar_vector;

// distance, course, and time utilities
double calculate_distance(const Point& a, const Point& b);
double calculate_course_deg(const Point& a, const Point& b);
int time_difference_minutes(const std::string& from, const std::string& to);

#endif //GEOMETRY_H