cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(stepper_control)

target_sources(app PRIVATE src/main.c)

if(CONFIG_STEPPER_MOCK)
    message(STATUS "Building with MOCKED stepper driver")
    target_sources(app PRIVATE src/stepper_driver_mock.c)
else()
    message(STATUS "Building with REAL stepper driver")
    target_sources(app PRIVATE src/stepper_driver.c)
endif()