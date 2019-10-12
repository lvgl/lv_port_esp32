cmake_minimum_required(VERSION 3.5)

add_definitions(-D LV_CONF_INCLUDE_SIMPLE)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(lvgl-demo)
