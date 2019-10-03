cmake_minimum_required(VERSION 3.5)

add_definitions(-D LV_CONF_INCLUDE_SIMPLE)
add_definitions(-D ILI9341_BCKL_ACTIVE_LVL=0)
add_definitions(-D TOUCH_SUPPORT=1)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ili9341-demo)
