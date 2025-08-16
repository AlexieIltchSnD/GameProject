#include "View.h"
#include <iomanip>
#include <iostream>
#include "Model.h"
#include "SimulationException.h"

// Parameter constructor
View::View(const double _scale, const int _size, const Point& _pan) {
    zoom(_scale);
    set_size(_size);
    span = _pan;
}

// Set map size, only if size > 6 and size <= 30,
// throw exception otherwise.
void View::set_size(const int _size) {
    if (_size > MIN_SIZE && _size <= MAX_SIZE)
        size = _size;
    else
        throw InvalidArgumentException("Error: Size is out of range");
}

// Set map zoom, only if zoom >= 1 and size <= 100,
// throw exception otherwise.
void View::zoom(const double _scale) {
    if (_scale >= MIN_SCALE && _scale <= MAX_SCALE ) {
        scale = _scale;
        is_scale_changed = true;
    }
    else
        throw InvalidArgumentException("Error: Scale is out of range");
}

// Set map pan.
void View::pan(const Point& _pan) {
    span = _pan;
}

// Revert to default map params.
void View::defaults() {
    scale = DEFAULT_SCALE;
    size = DEFAULT_SIZE;
    span = Point(DEFAULT_PAN,DEFAULT_PAN);
    is_scale_changed = false;
}

// Show the game map.
void View::show() const {
    std::cout << "Display size: "  << size << ", scale: " << scale << ", origin: (" << span.x << ", " << span.y << ")" << std::endl;

    vector<std::vector<std::string>> grid(size, std::vector<std::string>(size, ". "));  // Vector of the map
    const auto& sim_objs = Model::get_instance().get_sim_list();

    const double offset_y = span.y , offset_x = span.x;
    for (const auto& obj : sim_objs) {
        const Point loc = obj->get_location();
        const int ix = is_scale_changed ? static_cast<int>(std::ceil((loc.x - offset_x) / scale)) : static_cast<int>((loc.x - offset_x) / scale);
        const int iy = is_scale_changed ? static_cast<int>(std::ceil((loc.y - offset_y) / scale)) : static_cast<int>((loc.y - offset_y) / scale);

        // if im within map range, add a label to the map otherwise an object shouldn't be visible.
        if (ix >= 0 && ix < size && iy >= 0 && iy < size) {
            std::string name = obj->get_name();
            const std::string label = name.substr(0, std::min<size_t>(2, name.size()));
            grid[iy][ix] = label;
        }
    }

    // Print grid top to bottom
    for (int row = size - 1; row >= 0; --row) {
        // Label every 3rd row with Y value
        if (row % 3 == 0) {
            const double y_val = span.y + row * scale;
            std::cout << std::setw(4) << static_cast<int>(y_val) << " ";
        } else {
            std::cout << "     ";
        }

        // Print the row
        for (int col = 0; col < size; ++col) {
            std::cout << grid[row][col];
        }
        std::cout << std::endl;
    }

    // print x-axis at the bottom
    std::cout << "   ";
    for (int col = 0; col < size; ++col) {
        if (col % 3 == 0) {
            const double x_val = span.x + col * scale;
            std::cout << std::setw(2) << static_cast<int>(x_val);
        } else {
            std::cout << "  ";
        }
    }
    std::cout << std::endl;
}