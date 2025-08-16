#include "Warehouse.h"
#include <iostream>

// Warehouse constructor.
Warehouse::Warehouse(const std::string &_name, const int _inventory, const float _x, const float _y) :
                    Sim_Obj(_name),inventory(_inventory), location(_x,_y) {}

// Returns the warehouse location on the map.
Point Warehouse::get_location() const {
    return location;
}

// Broadcast warehouse state.
void Warehouse::broadcast_current_state() const {
    std::cout << "Warehouse " << get_name() << " at position ";
    get_location().print();
    std::cout << ", Inventory: " << inventory << std::endl;
}

// Function that receives crate amount and bool add
// if add is true, then add crates to inventory, subtract otherwise.
void Warehouse::update_inventory(const int _update_val, const bool add) {
    inventory = add ? inventory + _update_val : inventory - _update_val;
}

// Make the warehouse as a main.
void Warehouse::mark_main_warehouse() {
    main_warehouse = true;
}

// Does nothing.
void Warehouse::update() {}