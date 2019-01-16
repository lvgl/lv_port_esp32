#
# Component Makefile
#



COMPONENT_SRCDIRS := . \
	lv_core \
	lv_draw \
	lv_objx \
	lv_hal \
	lv_misc \
	lv_misc/lv_fonts \
	lv_themes \
	lv_fonts
COMPONENT_ADD_INCLUDEDIRS := $(COMPONENT_SRCDIRS) ..
