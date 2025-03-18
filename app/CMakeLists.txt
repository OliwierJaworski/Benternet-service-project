cmake_minimum_required(VERSION 3.10.0)
project(test VERSION 0.1.0 LANGUAGES C CXX)

find_package(cppzmq)

add_library(COMMON_LIBS INTERFACE)
target_link_libraries(COMMON_LIBS INTERFACE cppzmq)

set(SUB_DIRS 
    "${CMAKE_CURRENT_SOURCE_DIR}/server"
    "${CMAKE_CURRENT_SOURCE_DIR}/client")

foreach(DIR IN LISTS SUB_DIRS)
    if (EXISTS "${DIR}" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
        add_subdirectory(${DIR})
    else()
        message(WARNING "my_add_subdirectory: Can't find ${DIR}/CMakeLists.txt")
    endif()
endforeach()


