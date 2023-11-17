cmake_minimum_required(VERSION 3.26)
project(lib)

include(FetchContent)
set(FETCHCONTENT_QUIET TRUE)

set(CF_FRAMEWORK_STATIC OFF)
set(CF_FRAMEWORK_SHARED ON)
set(CF_FRAMEWORK_BUILD_TESTS OFF)
set(CF_FRAMEWORK_BUILD_SAMPLES OFF)
fetchcontent_declare(
        cute
        GIT_REPOSITORY https://github.com/RandyGaul/cute_framework.git
        GIT_PROGRESS FALSE
        SYSTEM
)
fetchcontent_makeavailable(cute)

fetchcontent_declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.12.2
        GIT_PROGRESS FALSE
        SYSTEM
)
fetchcontent_makeavailable(entt)

set(FLECS_SHARED ON)
set(FLECS_STATIC OFF)
fetchcontent_declare(
        flecs
        GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
        GIT_TAG master
        GIT_PROGRESS FALSE
        SYSTEM
)
fetchcontent_makeavailable(flecs)

#fetchcontent_declare(
#        fmt
#        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#        GIT_TAG 10.1.1
#        GIT_PROGRESS FALSE
#        SYSTEM
#)
#fetchcontent_makeavailable(fmt)

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

add_library(cute_headers STATIC src/cute_headers.cpp)
target_include_directories(cute_headers SYSTEM PUBLIC ${cute_headers_SOURCE_DIR})

set(lib_INCLUDE_DIRS
    ${entt_SOURCE_DIR}/src
    ${cute_SOURCE_DIR}/include
    ${cute_headers_SOURCE_DIR}
    ${flecs_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

