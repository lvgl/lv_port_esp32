#
# Component Makefile
#



COMPONENT_SRCDIRS := . \
	src/lv_core \
	src/lv_draw \
	src/lv_objx \
	src/lv_hal \
	src/lv_misc \
	src/lv_themes \
	src/lv_font
COMPONENT_ADD_INCLUDEDIRS := $(COMPONENT_SRCDIRS) ..
