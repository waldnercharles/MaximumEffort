add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0 _CRT_SECURE_NO_WARNINGS)
add_compile_options(-nologo -fp:fast)

add_executable(http_server
               http_server.cpp
               http_server.h
               http_server_main.cpp
               httplib.h
)

target_link_libraries(http_server ${PROJECT_NAME}_static)