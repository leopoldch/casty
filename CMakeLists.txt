cmake_minimum_required(VERSION 3.10)

project(Casty)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(BLUEZ REQUIRED bluez)

include_directories(
    ${BLUEZ_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}
)
file(GLOB_RECURSE SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/main.c
)

add_executable(castyd ${SRC_FILES})

target_link_libraries(castyd PRIVATE ${BLUEZ_LIBRARIES})
target_compile_options(castyd PRIVATE ${BLUEZ_CFLAGS_OTHER})
