cmake_minimum_required(VERSION 3.26)
project(MaximumEffort)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0 _CRT_SECURE_NO_WARNINGS)

if (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(EMSCRIPTEN TRUE)
endif ()

include(FetchContent)
set(FETCHCONTENT_QUIET TRUE)

add_executable(MaximumEffort
               include/aabb_grid.h
               include/array_algorithms.h
               include/assets.h
               include/cmp/debug_draw_circle_component.h
               include/cmp/enemy_component.h
               include/cmp/enemy_spawner_component.h
               include/cmp/hitbox_component.h
               include/cmp/hurtbox_component.h
               include/cmp/input_component.h
               include/cmp/lifetime_component.h
               include/cmp/movement_behavior_constant_direction_component.h
               include/cmp/movement_behavior_follow_target_component.h
               include/cmp/movement_component.h
               include/cmp/player_component.h
               include/cmp/projectile_component.h
               include/cmp/sprite_component.h
               include/cmp/transform_component.h
               include/cmp/weapon_component.h
               include/common.h
               include/enemy_type.h
               include/game.h
               include/log.h
               include/prefabs/player_prefab.h
               include/projectile_factories.h
               include/singleton.h
               include/states/game_state.h
               include/states/game_state_main_menu.h
               include/states/game_state_playing.h
               include/sys/camera_system.h
               include/sys/input_system.h
               include/sys/lifetime_system.h
               include/sys/movement_behavior_system.h
               include/sys/movement_system.h
               include/sys/physics_system.h
               include/sys/player_animation_system.h
               include/sys/render_system.h
               include/sys/spawner_system.h
               include/sys/system.h
               include/sys/weapon_system.h
               include/tiled_map.h
               src/assets.cpp
               src/cmp/transform_component.cpp
               src/game.cpp
               src/main.cpp
               src/prefabs/player_prefab.cpp
               src/projectile_factories.cpp
               src/states/game_state_main_menu.cpp
               src/states/game_state_playing.cpp
               src/sys/camera_system.cpp
               src/sys/input_system.cpp
               src/sys/lifetime_system.cpp
               src/sys/movement_behavior_system.cpp
               src/sys/movement_system.cpp
               src/sys/physics_system.cpp
               src/sys/player_animation_system.cpp
               src/sys/render_system.cpp
               src/sys/spawner_system.cpp
               src/sys/weapon_system.cpp
               src/tiled_map.cpp
               src/sys/movement_behavior_system.cpp
               src/prefabs/enemy_spawner_prefab.cpp
               include/prefabs/enemy_spawner_prefab.h
               src/prefabs/enemy_eyeball_prefab.cpp
               include/prefabs/enemy_eyeball_prefab.h
               src/prefabs/weapon_boomerang_prefab.cpp
               include/prefabs/weapon_boomerang_prefab.h
               include/prefabs/projectile_boomerang_prefab.h
               src/prefabs/projectile_boomerang_prefab.cpp
)

add_subdirectory(lib)

target_include_directories(MaximumEffort SYSTEM PRIVATE ${lib_INCLUDE_DIRS})
target_include_directories(MaximumEffort SYSTEM PRIVATE ${entt_SOURCE_DIR}/single_include)
target_include_directories(MaximumEffort PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(MaximumEffort cute cute_headers fmt)

add_custom_target(copy-assets ALL
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)

add_dependencies(MaximumEffort copy-assets)

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

# Statically link the standard library when using MinGW, because it's dumb.
if (WIN32 AND NOT MSVC)
    target_link_options(MaximumEffort PRIVATE -static-libgcc -static-libstdc++)
    target_link_libraries(MaximumEffort ws2_32 secur32 shlwapi)
endif ()

if (MSVC)
    set_property(TARGET MaximumEffort PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:MaximumEffort>)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MaximumEffort)

    target_compile_options(${PROJECT_NAME} PRIVATE -WX -W4 -wd4100 -wd4189 -wd4267 -wd4244 -wd4458 -wd4459)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/profile")
endif ()
