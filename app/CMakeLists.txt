cmake_minimum_required(VERSION 3.10.0)
project(test VERSION 0.1.0 LANGUAGES C CXX)

IF(WIN32)
    set(ZMQ_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/zmq")
    set(ZMQ_INCLUDE_DIR "${ZMQ_ROOT}/include")
    set(ZMQ_LIB_DIR "${ZMQ_ROOT}/lib")
ENDIF (WIN32)

add_library(COMMON_LIBS INTERFACE)

target_include_directories(COMMON_LIBS INTERFACE ${ZMQ_INCLUDE_DIR})

target_link_libraries(COMMON_LIBS INTERFACE
    zmq 
    zmqpp
)

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


