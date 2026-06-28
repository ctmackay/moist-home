#include "MqttClient.hpp"
#include "PlantController.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main() {
    PlantController controller;
    MqttClient mqtt("plant-controller", "localhost", 1883);

    mqtt.setCommandCallback([&controller](const std::string& command) {
        controller.handleCommand(command);
    });

    if (!mqtt.connect()) {
        return 1;
    }

    mqtt.subscribe("plant/commands");
    mqtt.startLoop();

    double fake_moisture = 15.0;

    while (true) {
        fake_moisture -= 1.0;

        if (fake_moisture < 0.0) {
            fake_moisture = 20.0;
        }

        controller.update(fake_moisture);

        std::string status =
            "moisture=" + std::to_string(fake_moisture) +
            ", pump_on=" + std::string(controller.isPumpOn() ? "true" : "false");
            
            mqtt.publish("plant/status", status);
            
            std::cout << "Published status: " << status << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
