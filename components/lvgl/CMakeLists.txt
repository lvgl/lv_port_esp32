file(GLOB_RECURSE SOURCES lvgl/src/*.c)
idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS . lvgl)

target_compile_definitions(${COMPONENT_LIB} INTERFACE LV_CONF_INCLUDE_SIMPLE=1)
