add_executable(${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        )

target_sources(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}/handle_connection.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/pracownia-4.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/request.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/stream.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/sys_wrapper.hpp
    PRIVATE
        handle_connection.cpp
        main.cpp
        request.cpp
        stream.cpp
        sys_wrapper.cpp
        )

add_subdirectory(responses)

target_link_libraries(${CMAKE_PROJECT_NAME}
        responses.obj)
