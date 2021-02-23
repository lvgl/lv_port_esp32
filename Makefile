#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := lvgl-demo

# Add new components (source folders)

# Any paths in this makefile should be absolute paths.
EXTRA_COMPONENT_DIRS := $(PROJECT_PATH)/components/lvgl_esp32_drivers/lvgl_tft
EXTRA_COMPONENT_DIRS += $(PROJECT_PATH)/components/lvgl_esp32_drivers/lvgl_touch

# Must be before include $(IDF_PATH)/make/project.mk
# $(PROJECT_PATH)/xxx didn't work -> use $(abspath xxx) instead

include $(IDF_PATH)/make/project.mk

