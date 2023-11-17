cmake_minimum_required(VERSION 3.26)
project(MaximumEffort)

cmake_policy(SET CMP0077 NEW)
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0 _CRT_SECURE_NO_WARNINGS) # FLECS_NDEBUG)

if (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(EMSCRIPTEN TRUE)
endif ()

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

add_executable(MaximumEffort
               include/array_algorithms.h
               include/assets.h
               include/camera.h
               include/cmp/boomerang_movement_component.h
               include/cmp/c_debug_draw_circle.h
               include/cmp/c_enemy_component.h
               include/cmp/c_enemy_spawner.h
               include/cmp/c_faction.h
               include/cmp/c_hitbox.h
               include/cmp/c_hurtbox.h
               include/cmp/c_input.h
               include/cmp/c_lifetime.h
               include/cmp/c_movement.h
               include/cmp/c_movement_behavior_constant_direction.h
               include/cmp/c_movement_behavior_follow_target.h
               include/cmp/c_physics.h
               include/cmp/c_player.h
               include/cmp/c_projectile.h
               include/cmp/c_sprite.h
               include/cmp/c_transform.h
               include/cmp/c_weapon.h
               include/common.h
               include/cute_aabb_grid.h
               include/enemy_type.h
               include/factories.h
               include/game.h
               include/projectile_factories.h
               include/sys/behavior_constant_direction_system.h
               include/sys/debug_draw_system.h
               include/sys/input_system.h
               include/sys/lifetime_system.h
               include/sys/behavior_follow_target_system.h
               include/sys/movement_system.h
               include/sys/physics_system.h
               include/sys/player_animation_system.h
               include/sys/render_system.h
               include/sys/spawner_system.h
               include/sys/transform_system.h
               include/sys/weapon_system.h
               include/tiled_map.h
               src/assets.cpp
               src/camera.cpp
               src/cmp/c_transform.cpp
               src/cute_aabb_grid.cpp
               src/factories.cpp
               src/game.cpp
               src/main.cpp
               src/projectile_factories.cpp
               src/sys/behavior_constant_direction_system.cpp
               src/sys/debug_draw_system.cpp
               src/sys/input_system.cpp
               src/sys/lifetime_system.cpp
               src/sys/behavior_follow_target_system.cpp
               src/sys/movement_system.cpp
               src/sys/physics_system.cpp
               src/sys/player_animation_system.cpp
               src/sys/render_system.cpp
               src/sys/spawner_system.cpp
               src/sys/transform_system.cpp
               src/sys/weapon_system.cpp
               src/tiled_map.cpp
               include/cute_ext.h
               include/aabb_grid.h
)

target_compile_features(MaximumEffort PRIVATE cxx_std_17)

add_subdirectory(lib)

target_include_directories(MaximumEffort SYSTEM PRIVATE ${lib_INCLUDE_DIRS})
target_include_directories(MaximumEffort SYSTEM PRIVATE ${entt_SOURCE_DIR}/single_include)
target_include_directories(MaximumEffort PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(MaximumEffort cute cute_headers flecs)

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
