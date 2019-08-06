#
# Component Makefile
#

CFLAGS += -DLV_CONF_INCLUDE_SIMPLE

COMPONENT_SRCDIRS := .      \
	lv_apps/benchmark         \
	lv_apps/demo              \
	lv_apps/sysmon            \
	lv_apps/terminal          \
	lv_apps/tpcal             \
	lv_test/lv_test_theme     \
	lv_test/lv_test_group     \
	lv_test/lv_test_group     \
	lv_tutorial/lv_test_group \
	lv_tutorial/10_keyboard   \
  lv_tutorial/1_hello_world \
  lv_tutorial/2_objects     \
  lv_tutorial/3_styles      \
  lv_tutorial/4_themes      \
  lv_tutorial/5_antialiasing  \
  lv_tutorial/6_images        \
  lv_tutorial/7_fonts         \
  lv_tutorial/8_animations    \
  lv_tutorial/9_responsive 

	
COMPONENT_ADD_INCLUDEDIRS := $(COMPONENT_SRCDIRS) ../
