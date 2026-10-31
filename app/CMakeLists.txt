# file: CMakeLists.txt | zephyr app build | rp2350 usb-c shell os
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(rp2350_os)

target_sources(app PRIVATE src/main.c)
