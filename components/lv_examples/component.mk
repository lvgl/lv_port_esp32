#
# Component Makefile
#

CFLAGS += -DLV_CONF_INCLUDE_SIMPLE

COMPONENT_SRCDIRS := lv_examples      \
    lv_examples/lv_apps/benchmark         \
    lv_examples/lv_apps/demo              \
    lv_examples/lv_apps/sysmon            \
    lv_examples/lv_apps/terminal          \
    lv_examples/lv_apps/tpcal             \
    lv_examples/lv_tests/lv_test_theme     \
    lv_examples/lv_tests/lv_test_group     \
    lv_examples/lv_tutorial/10_keyboard   \
    lv_examples/lv_tutorial/1_hello_world \
    lv_examples/lv_tutorial/2_objects     \
    lv_examples/lv_tutorial/3_styles      \
    lv_examples/lv_tutorial/4_themes      \
    lv_examples/lv_tutorial/5_antialiasing  \
    lv_examples/lv_tutorial/6_images        \
    lv_examples/lv_tutorial/7_fonts         \
    lv_examples/lv_tutorial/8_animations    \
    lv_examples/lv_tutorial/9_responsive


COMPONENT_ADD_INCLUDEDIRS := $(COMPONENT_SRCDIRS) . ..
