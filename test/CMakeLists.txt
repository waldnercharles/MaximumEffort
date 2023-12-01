include(FetchContent)
set(FETCHCONTENT_QUIET TRUE)

add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0 _CRT_SECURE_NO_WARNINGS)

fetchcontent_declare(
        pico_headers
        GIT_REPOSITORY https://github.com/empyreanx/pico_headers.git
        GIT_TAG main
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        SYSTEM
)

fetchcontent_getproperties(pico_headers)
fetchcontent_populate(pico_headers)

add_executable(test
               pico_headers.cpp
               main.cpp
               test_event_system.cpp
               test_csv.cpp
)

target_include_directories(test SYSTEM PUBLIC ${pico_headers_SOURCE_DIR})
target_link_libraries(test ${PROJECT_NAME}_static)