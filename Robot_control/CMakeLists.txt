cmake_minimum_required(VERSION 2.8)
project(main)
find_package( OpenCV REQUIRED )

find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto REQUIRED IMPORTED_TARGET libmosquitto)


include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(main 
    "main.cpp"
    "Definer.cpp"
    "Robot.cpp"
    "Robot_control.cpp"
    "Drink_control.cpp"
    "Order_control.cpp"
    "Mqtt.cpp")

target_link_libraries( main ${OpenCV_LIBS}
    PkgConfig::Mosquitto )
