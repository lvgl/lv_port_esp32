#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := hello-world

# Add new components (source folders)
# Must be before include $(IDF_PATH)/make/project.mk
# $(PROJECT_PATH)/xxx didn't work -> use $(abspath xxx) instead
EXTRA_COMPONENT_DIRS := $(abspath drv)	$(abspath lvgl) $(abspath lv_examples)

include $(IDF_PATH)/make/project.mk

