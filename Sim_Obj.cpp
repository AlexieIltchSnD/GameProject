#include "Sim_Obj.h"

Sim_Obj::Sim_Obj(std::string  _name): name(std::move(_name)){}

std::string Sim_Obj::get_name() const {
    return name;
}