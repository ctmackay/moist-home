#pragma once

#include <string>

enum class PlantState {
    Idle,
    Watering,
    Error
};

class PlantController {
public:
    void update(double moisture);
    void handleCommand(const std::string& command);

    bool isPumpOn() const;
    PlantState state() const;

private:
    void startWatering();
    void stopWatering();

    double moisture_threshold_ = 10.0;
    bool pump_on_ = false;
    PlantState state_ = PlantState::Idle;
};
