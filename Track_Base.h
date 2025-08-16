#ifndef TRACK_BASE_H
#define TRACK_BASE_H

#include "Geometry.h"

/**
 * Track_Base class
 * Manages basic tracking data (position, speed, course) for a moving object.
 * Provides setters and getters for track data.
 */
class Track_Base{
public:
    Track_Base(double _course, double _speed, const Point& _position);  // Constructor.
    Point get_position() const;     // Get position.
    double get_course() const;      // Get course.
    double get_speed() const;       // Get speed.

    void add_position(double x, double y);               // Adjust position by offset.
    void set_parameters(double _speed, double _course);  // Set speed and course.
    void set_course(double _course);                     // Set course.
    void set_position(const Point& _position);           // Set position.
    void set_speed(double _speed);                       // Set speed.

private:
    double course;      // Current course.
    double speed;       // Current speed.
    Point position;     // Current position.
};
#endif //TRACK_BASE_H