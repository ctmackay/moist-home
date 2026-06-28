#include "PlantController.hpp"

#include <iostream>

void PlantController::update(double moisture) {
    if (state_ == PlantState::Error) {
        stopWatering();
        return;
    }

    if (moisture < moisture_threshold_ && !pump_on_) {
        startWatering();
    }

    if (moisture >= moisture_threshold_ && pump_on_) {
        stopWatering();
    }
}

void PlantController::handleCommand(const std::string& command) {
    if (command == "water") {
        startWatering();
    } else if (command == "stop") {
        stopWatering();
    } else {
        std::cout << "Unknown command:"  << command << std::endl;
    }
}

bool PlantController::isPumpOn() const {
    return pump_on_;
}

PlantState PlantController::state() const {
    return state_;
}

void PlantController::startWatering() {
    pump_on_ = true;
    state_ = PlantState::Watering;

    std::cout << "Pump ON" << std::endl;

    // Later: GPIO relay ON
}

void PlantController::stopWatering() {
    pump_on_ = false;
    state_ = PlantState::Idle;

    std::cout << "Pump OFF" << std::endl;

    // Later: GPIO relay OFF
}
