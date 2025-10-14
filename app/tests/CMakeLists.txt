cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(stepper_tests)

# Include the source files but not main.c
target_sources(app PRIVATE
    src/test_stepper.c
    ../src/stepper_driver.c
)

target_include_directories(app PRIVATE ../src)