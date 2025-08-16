#ifndef VIEW_H
#define VIEW_H
#include "Geometry.h"
#define MAX_SIZE 30
#define MIN_SIZE 6
#define MAX_SCALE 100
#define MIN_SCALE 1
#define DEFAULT_SCALE 2.0
#define DEFAULT_SIZE 25
#define DEFAULT_PAN (-10)

/**
 * View class, Displays the game map with given parameters.
 */
class View {
public:
    View() = default;   // Default ctor.
    View(double _scale,int _size, const Point& _pan); // Parameter ctor.

    void set_size(int _size);   // Set map size.
    void zoom(double _scale);   // Set map zoom.
    void pan(const Point& _pan);// set pan.
    void defaults();            // Set default parameters.
    void show() const;          // Show the map.

private:
    double scale = DEFAULT_SCALE;                      // Map scale.
    int size = DEFAULT_SIZE;                          // Map size.
    Point span = Point(DEFAULT_PAN,DEFAULT_PAN); // Map pan.
    bool is_scale_changed = false;
};

#endif //VIEW_H