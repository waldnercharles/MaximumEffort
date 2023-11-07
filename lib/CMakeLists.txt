cmake_minimum_required(VERSION 3.26)
project(lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(FETCHCONTENT_QUIET TRUE)

if (MSVC)
    add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0 ENTT_DISABLE_ASSERT)
    add_compile_options(-nologo -fp:fast)
endif ()

set(CUTE_FRAMEWORK_STATIC ON)
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

add_library(cute_headers STATIC
            src/cute_headers.cpp
)
target_include_directories(cute_headers SYSTEM PUBLIC ${cute_headers_SOURCE_DIR})

set(lib_INCLUDE_DIRS ${entt_SOURCE_DIR}/single_include ${cute_headers_SOURCE_DIR} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

