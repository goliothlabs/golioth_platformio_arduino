set (deps
     "golioth_sdk"
     "nvs_flash"
     "json"
)

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

idf_component_register(
     SRCS ${app_sources}
     PRIV_REQUIRES ${deps}
)
