cmake_minimum_required(VERSION 3.15)
project(pracownia-4
        VERSION 0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ${CMAKE_HOME_DIRECTORY}/include)

add_subdirectory(src)
#add_subdirectory(test)

include_directories(${CMAKE_SOURCE_DIR}/include)

execute_process(COMMAND bash -c "tar xf ${CMAKE_HOME_DIRECTORY}/webpages.tgz")
