# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(nerfdome C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    src/main.c
    src/modules/servo.c
    src/modules/serial.c
    src/btstack/spp.c
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_pwm
    hardware_timer
    hardware_uart
    pico_btstack_ble
    pico_btstack_classic
    pico_btstack_cyw43
    pico_cyw43_arch_none
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include/btstack
    include/modules
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)