cmake_minimum_required(VERSION 3.26)
project(MaximumEffort)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(EMSCRIPTEN TRUE)
endif ()

add_subdirectory(src)
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_static)

add_subdirectory(test)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_compile_options(${PROJECT_NAME}_static PUBLIC -O1 -fno-rtti -fno-exceptions)
    target_link_options(${PROJECT_NAME} PUBLIC -o ${PROJECT_NAME}.html --preload-file ${CMAKE_SOURCE_DIR}/content@/content --emrun -s ASYNCIFY=1 -O1)
endif ()

if (APPLE)
    set_target_properties(${PROJECT_NAME}_static PROPERTIES
                          MACOSX_BUNDLE_GUI_IDENTIFIER "com.Maximum.Effort"
                          MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
                          MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
    )
endif ()

# Statically link the standard library when using MinGW, because it's dumb.
if (WIN32 AND NOT MSVC)
    target_link_options(${PROJECT_NAME} PUBLIC -static-libgcc -static-libstdc++)
    target_link_libraries(${PROJECT_NAME} ws2_32 secur32 shlwapi)
endif ()

if (MSVC)
    add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0 _CRT_SECURE_NO_WARNINGS)
    add_compile_options(-nologo -fp:fast)

    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

    target_compile_options(${PROJECT_NAME}_static PUBLIC -WX -W4 -wd4100 -wd4189 -wd4267 -wd4244 -wd4458 -wd4459)
    target_compile_options(${PROJECT_NAME} PUBLIC -WX -W4 -wd4100 -wd4189 -wd4267 -wd4244 -wd4458 -wd4459)
    set_target_properties(${PROJECT_NAME}_static PROPERTIES LINK_FLAGS "/profile")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/profile")
endif ()
