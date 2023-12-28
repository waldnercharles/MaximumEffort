include(FetchContent)
set(FETCHCONTENT_QUIET TRUE)

if (MSVC)
    add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0 _CRT_SECURE_NO_WARNINGS)
    add_compile_options(-nologo -fp:fast)
endif ()

set(CF_FRAMEWORK_STATIC ON)
set(CF_FRAMEWORK_BUILD_TESTS OFF)
set(CF_FRAMEWORK_BUILD_SAMPLES OFF)
fetchcontent_declare(
        cute
        GIT_REPOSITORY https://github.com/RandyGaul/cute_framework.git
        GIT_PROGRESS FALSE
        SYSTEM
)
fetchcontent_makeavailable(cute)

#add_compile_definitions(ENTT_DISABLE_ASSERT)
fetchcontent_declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.12.2
        GIT_PROGRESS FALSE
        SYSTEM
)
fetchcontent_makeavailable(entt)

fetchcontent_declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.1.1
        GIT_PROGRESS FALSE
        SYSTEM
)
fetchcontent_makeavailable(fmt)

fetchcontent_declare(
        cute_headers
        GIT_REPOSITORY https://github.com/RandyGaul/cute_headers.git
        GIT_TAG 265f675325629f3bae5710c6f1362bba9f619357
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        SYSTEM
)

fetchcontent_getproperties(cute_headers)
fetchcontent_populate(cute_headers)

add_library(${PROJECT_NAME}_static STATIC
            aabb_grid.h
            assets.cpp
            assets.h
            cmp/age_component.h
            cmp/bullet_emitter_component.h
            cmp/damage_component.h
            cmp/debug_draw_circle_component.h
            cmp/drop_xp_component.h
            cmp/enemy_component.h
            cmp/enemy_spawner_component.h
            cmp/facing_component.h
            cmp/frame_destroy_component.h
            cmp/health_component.h
            cmp/hit_component.h
            cmp/hit_component.h
            cmp/hit_immunity_component.h
            cmp/hitbox_component.h
            cmp/homing_mover_component.h
            cmp/hurtbox_component.h
            cmp/input_component.h
            cmp/lifetime_component.h
            cmp/movement_behavior_constant_direction_component.h
            cmp/movement_behavior_follow_target_component.h
            cmp/movement_component.h
            cmp/physics_component.h
            cmp/pickup_collector_component.h
            cmp/pickup_component.h
            cmp/player_component.h
            cmp/projectile_component.h
            cmp/sprite_component.h
            cmp/stats_component.h
            cmp/transform_component.cpp
            cmp/transform_component.h
            cmp/xp_component.h
            common.h
            event_handlers/damage_numbers_event_handler.cpp
            event_handlers/damage_numbers_event_handler.h
            event_handlers/death_event_handler.cpp
            event_handlers/death_event_handler.h
            event_handlers/hit_event_handler.cpp
            event_handlers/hit_event_handler.h
            events/damage_event.h
            events/death_event.h
            events/event_bus.h
            events/hit_event.h
            events/subscription.h
            events/type_id.h
            factories/enemy_factory.cpp
            factories/enemy_factory.h
            game.cpp
            game.h
            game_timer.cpp
            game_timer.h
            level_1.cpp
            level_1.h
            lib/cute_headers.cpp
            log.h
            prefabs/enemy_spawner_prefab.cpp
            prefabs/enemy_spawner_prefab.h
            prefabs/player_prefab.cpp
            prefabs/player_prefab.h
            prefabs/projectile_prefab.cpp
            prefabs/projectile_prefab.h
            prefabs/weapon_prefab.cpp
            prefabs/weapon_prefab.h
            rendering/render_target.cpp
            rendering/render_target.h
            serialization/json.cpp
            serialization/json.h
            shaders/blit_shader.h
            states/game_state.h
            states/game_state_main_menu.cpp
            states/game_state_main_menu.h
            states/game_state_playing.cpp
            states/game_state_playing.h
            sys/animation_system.cpp
            sys/animation_system.h
            sys/camera_system.cpp
            sys/camera_system.h
            sys/difficulty_system.cpp
            sys/difficulty_system.h
            sys/health_system.cpp
            sys/health_system.h
            sys/hit_immunity_system.cpp
            sys/hit_immunity_system.h
            sys/homing_mover_system.cpp
            sys/homing_mover_system.h
            sys/input_system.cpp
            sys/input_system.h
            sys/lifetime_system.cpp
            sys/lifetime_system.h
            sys/movement_behavior_system.cpp
            sys/movement_behavior_system.cpp
            sys/movement_behavior_system.h
            sys/movement_system.cpp
            sys/movement_system.h
            sys/physics_system.cpp
            sys/physics_system.h
            sys/pickup_system.cpp
            sys/pickup_system.h
            sys/projectile_system.cpp
            sys/projectile_system.h
            sys/render_system.cpp
            sys/render_system.h
            sys/spawner_system.cpp
            sys/spawner_system.h
            sys/system.h
            sys/weapon_system.cpp
            sys/weapon_system.h
            sys/xp_system.cpp
            sys/xp_system.h
            tiled/tiled_map.cpp
            tiled/tiled_map.h
            events/pickup_collected_event.h
            cmp/level_component.h
            events/xp_gained_event.h
            events/level_up_event.h
            states/game_state_loading_level.cpp
            states/game_state_loading_level.h
            passive_tree.h
            passive_tree.cpp
            cmp/modifiers_component.h
            http/httplib.h
            http/http_server.cpp
            http/http_server.h
            event_handlers/level_up_event_handler.cpp
            event_handlers/level_up_event_handler.h
            leveling_overlay.cpp
            leveling_overlay.h
)

target_include_directories(${PROJECT_NAME}_static SYSTEM PUBLIC ${cute_headers_SOURCE_DIR} ${entt_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME}_static PUBLIC .)

target_link_libraries(${PROJECT_NAME}_static cute fmt)

add_custom_target(always_run ALL
                  DEPENDS
                  copy_assets
)

add_custom_command(
        OUTPUT copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
        DEPENDS
        always_rebuild
)

add_custom_command(
        OUTPUT always_rebuild
        COMMAND cmake -E echo
)

add_dependencies(${PROJECT_NAME}_static always_run)