file(GLOB_RECURSE SOURCES lvgl/src/*.c)
idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS ../ lvgl/ .)
