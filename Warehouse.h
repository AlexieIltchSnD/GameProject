#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "Sim_Obj.h"
/**
 * Warehouse class, extends Sim_Obj
 * Simulates a warehouse at a static position on the map.
 */
class Warehouse final : public Sim_Obj{
public:
    explicit Warehouse(const std::string& _name,int _inventory, float _x, float _y);    // Explicit ctor

    Point get_location() const override;               // Location of warehouse getter.
    void broadcast_current_state() const override;     // Broadcast warehouse state.
    void update_inventory(int _update_val, bool add);  // Update warehouse inventory.
    void mark_main_warehouse();                        // Is this warehouse the first one?.
    void update() override;                            // Update warehouse state.

private:
    int inventory;                  // Warehouse inventory.
    bool main_warehouse = false;    // Is this warehouse the main one.
    Point location;                 // Warehouse location
};

#endif //WAREHOUSE_H