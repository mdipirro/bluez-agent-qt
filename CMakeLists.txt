cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)

project(BluetoothAgent VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core DBus)

add_executable(${PROJECT_NAME}
    "main.cpp"
    "pairingagentwithvalidation.h"
    "pairingagentwithvalidation.cpp"
    "btmacaddressagent.h"
    "btmacaddressagent.cpp"
    "adaptor.cpp"
    "adaptor.h"
)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::DBus)
