add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        )

#target_sources(${CMAKE_PROJECT_NAME}
#    PUBLIC
#        ${CMAKE_INCLUDE_CURRENT_DIR}/pracownia-4.hpp
#        ${CMAKE_INCLUDE_CURRENT_DIR}/request.hpp
#    PRIVATE
#        pracownia-4.cpp
#        main.cpp
#        request.cpp
#        )
