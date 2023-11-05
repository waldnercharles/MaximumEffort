cmake_minimum_required(VERSION 3.26)
project(MaximumEffort)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(EMSCRIPTEN TRUE)
endif ()

include(FetchContent)
set(FETCHCONTENT_QUIET TRUE)

set(CUTE_FRAMEWORK_STATIC ON)
fetchcontent_declare(
        cute
        GIT_REPOSITORY https://github.com/RandyGaul/cute_framework.git
        GIT_PROGRESS FALSE
)
fetchcontent_makeavailable(cute)

fetchcontent_declare(
        cute_headers
        GIT_REPOSITORY https://github.com/RandyGaul/cute_headers.git
        GIT_TAG master
        CONFIGURE_COMMAND ""
        BUILD_COMMAND "")

fetchcontent_getproperties(cute_headers)
fetchcontent_populate(cute_headers)

set(cute_headers_INCLUDE_DIRS ${cute_headers_SOURCE_DIR} CACHE INTERNAL "cute_headers: Include Directories" FORCE)
add_library(cute_headers STATIC ${cute_headers_SOURCE_DIR}/cute_tiled.h src/cute_headers.cpp)

fetchcontent_declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.1.1
        GIT_PROGRESS FALSE
)
fetchcontent_makeavailable(fmt)

add_executable(MaximumEffort
               include/assets.h
               include/common.h
               include/enemy.h
               include/enemy_spawner.h
               include/enemy_type.h
               include/game.h
               include/log.h
               include/player.h
               include/projectile.h
               include/singleton.h
               include/tiled_map.h
               include/weapon.h
               src/assets.cpp
               src/enemy.cpp
               src/enemy_spawner.cpp
               src/game.cpp
               src/main.cpp
               src/player.cpp
               src/projectile.cpp
               src/tiled_map.cpp
               src/weapon.cpp
)

include_directories(SYSTEM ${cute_headers_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_compile_options(MaximumEffort PUBLIC -O1 -fno-rtti -fno-exceptions)
    target_link_options(MaximumEffort PRIVATE -o MaximumEffort.html --preload-file ${CMAKE_SOURCE_DIR}/content@/content --emrun -s ASYNCIFY=1 -O1)
endif ()

if (APPLE)
    set_target_properties(MaximumEffort PROPERTIES
                          MACOSX_BUNDLE_GUI_IDENTIFIER "com.Maximum.Effort"
                          MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
                          MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
    )
endif ()

target_link_libraries(MaximumEffort cute cute_headers fmt)

if (MSVC)
    set_property(TARGET MaximumEffort PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:MaximumEffort>)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MaximumEffort)
endif ()
