cmake_minimum_required(VERSION 3.10.0)
project(server_zmq VERSION 0.1.0 LANGUAGES C CXX)

option(CURL_STATIC_LINKING "Set to ON to build libcurl with static linking."  OFF)

if(CURL_STATIC_LINKING)
    message("-DCURL_STATICLIB [added]")
    add_definitions(-DCURL_STATICLIB)
endif()

add_definitions(-DJSON_USE_IMPLICIT_CONVERSIONS=0)

find_package(CURL REQUIRED)

set(SRC_FILES "")
file(GLOB ALL_CONTENTS "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(DIR IN LISTS ALL_CONTENTS)
     if (EXISTS "${DIR}")
         get_filename_component(EXT ${DIR} EXT)
         if(EXT STREQUAL ".cpp" OR EXT STREQUAL ".h")
             list(APPEND SRC_FILES "${DIR}")
             message(STATUS "Found file: ${DIR}")
         endif()
     endif()
 endforeach()

add_executable(${PROJECT_NAME}  
    server.cpp
    ${SRC_FILES}
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CURL_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME} 
    COMMON_LIBS
    ${CURL_LIBRARIES}
    )
