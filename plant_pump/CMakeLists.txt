cmake_minimum_required(VERSION 3.10)

project(plant_pump_controller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(plant_controller
    src/main.cpp
    src/PlantController.cpp
    src/MqttClient.cpp
)

target_include_directories(plant_controller PRIVATE
    include
)

target_link_libraries(plant_controller PRIVATE
    mosquitto
    pthread
)
