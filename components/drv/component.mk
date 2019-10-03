# drv 

CFLAGS += -DLV_CONF_INCLUDE_SIMPLE
CFLAGS += -DILI9341_BCKL_ACTIVE_LVL=0

# Set to 1 if your display have touch support, otherwise set it to 0.
CFLAGS += -DTOUCH_SUPPORT=1

COMPONENT_SRCDIRS := . 
	
COMPONENT_ADD_INCLUDEDIRS := $(COMPONENT_SRCDIRS) . ../ ../lvgl
