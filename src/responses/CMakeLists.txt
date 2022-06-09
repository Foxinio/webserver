set(MODULE_NAME responses.obj)

add_library(${MODULE_NAME} OBJECT)

target_include_directories(${MODULE_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        )

target_sources(${MODULE_NAME}
    PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/fwd.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/include_file.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response_forbidden.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response_moved_permanently.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response_not_found.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response_not_implemented.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response_ok.hpp
    PRIVATE
        response.cpp
        include_file.cpp
        response_forbidden.cpp
        response_moved_permanently.cpp
        response_not_found.cpp
        response_not_implemented.cpp
        response_ok.cpp
        )
