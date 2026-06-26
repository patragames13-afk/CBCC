cmake_minimum_required(VERSION 3.16)
project(cbc_ballistics VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(cbc_ballistics
    src/main.cpp
    src/calculator.cpp
    src/cli.cpp
    src/data.cpp
    src/physics.cpp
    src/physics_dual.cpp
    src/gui.cpp
    src/gui_css.cpp
    src/gui_layout.cpp
    src/gui_output.cpp
    src/gui_draw.cpp
    src/gui_handlers.cpp
    src/gui_actions.cpp
)

target_include_directories(cbc_ballistics PRIVATE src)
target_link_libraries(cbc_ballistics PRIVATE ${GTK3_LIBRARIES})
target_include_directories(cbc_ballistics PRIVATE ${GTK3_INCLUDE_DIRS})
target_compile_options(cbc_ballistics PRIVATE ${GTK3_CFLAGS_OTHER})
