#ifndef SIMULATIONEXCEPTION_H
#define SIMULATIONEXCEPTION_H

#include <exception>
#include <string>

/**
 * SimulationException hierarchy
 * Base exception class for simulation-specific errors and various derived exceptions.
 * Each derived class represents a specific error type in the simulation context.
 */
class SimulationException : public std::exception {
    std::string message;
public:
    explicit SimulationException(std::string  message) : message(std::move(message)) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Derived exceptions for specific simulation error types.
class InvalidFileArgumentsException final : public SimulationException {
public:
    explicit InvalidFileArgumentsException(const std::string& what) : SimulationException(what) {};
};

class InvalidFlagsException final : public SimulationException {
public:
    explicit InvalidFlagsException(const std::string& what) : SimulationException(what) {};
};

class InvalidCommandException final : public SimulationException {
public:
    explicit InvalidCommandException(const std::string& what) : SimulationException(what) {};
};

class IllegalNameException final : public SimulationException {
public:
    explicit IllegalNameException(const std::string& what) : SimulationException(what) {};
};

class BadCoordinatesException final : public SimulationException {
public:
    explicit BadCoordinatesException(const std::string& what) : SimulationException(what) {};
};

class InvalidTypeException final : public SimulationException {
public:
    explicit InvalidTypeException(const std::string& what) : SimulationException(what) {};
};

class InvalidArgumentsForTypeException final : public SimulationException {
public:
    explicit InvalidArgumentsForTypeException(const std::string& what) : SimulationException(what) {};
};

class WarehouseNotFoundException final : public SimulationException {
public:
    explicit WarehouseNotFoundException(const std::string& what) : SimulationException(what) {};
};

class VehicleNotFoundException final : public SimulationException {
public:
    explicit VehicleNotFoundException(const std::string& what) : SimulationException(what) {};
};

class NotFoundException final : public SimulationException {
public:
    explicit NotFoundException(const std::string& what) : SimulationException(what) {};
};

class InvalidCommandFormatException final : public SimulationException {
public:
    explicit InvalidCommandFormatException(const std::string& what) : SimulationException(what) {};
};

class FileException final : public SimulationException {
public:
    explicit FileException(const std::string& what) : SimulationException(what) {};
};

class InvalidInputLineException final : public SimulationException {
public:
    explicit InvalidInputLineException(const std::string& what) : SimulationException(what) {};
};

class InvalidArgumentException final : public SimulationException {
public:
    explicit InvalidArgumentException(const std::string& what) : SimulationException(what) {};
};

class TrooperNearbyException final : public SimulationException {
public:
    explicit TrooperNearbyException(const std::string& what) : SimulationException(what) {};
};

class AttackFailedException final : public SimulationException {
public:
    explicit AttackFailedException(const std::string& what) : SimulationException(what) {};
};
#endif //SIMULATIONEXCEPTION_H
