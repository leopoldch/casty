cmake_minimum_required(VERSION 3.10)

project(Casty)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(BLUEZ REQUIRED bluez)

include_directories(
    ${BLUEZ_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/bluetooth 
)

file(GLOB_RECURSE SRC_FILES
    ${CMAKE_SOURCE_DIR}/bluetooth/*.c
)

# Définir l'exécutable principal
add_executable(main ${SRC_FILES})

# Lier les bibliothèques
target_link_libraries(main PRIVATE ${BLUEZ_LIBRARIES})

# Ajouter les options de compilation
target_compile_options(main PRIVATE ${BLUEZ_CFLAGS_OTHER})
