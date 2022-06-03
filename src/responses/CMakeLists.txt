set(MODULE_NAME responses.obj)

add_library(${MODULE_NAME} OBJECT)

target_include_directories(${MODULE_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        )

target_sources(${MODULE_NAME}
    PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/fwd.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response_200.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response_301.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response_403.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response_404.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/responses/response_501.hpp
    PRIVATE
        response.cpp
        response_200.cpp
        response_301.cpp
        response_403.cpp
        response_404.cpp
        response_501.cpp
        )
