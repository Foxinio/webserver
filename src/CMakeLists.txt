add_executable(${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        )

target_sources(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${CMAKE_INCLUDE_CURRENT_DIR}/pracownia-4.hpp
        ${CMAKE_INCLUDE_CURRENT_DIR}/request.hpp
    PRIVATE
        main.cpp
        request.cpp
        )

add_subdirectory(responses)

target_link_libraries(${CMAKE_PROJECT_NAME}
        responses.obj)
